/***************************************************************************
 * auction.c — Quest-Point Auction House for GodWars Modernized            *
 *                                                                         *
 * Players auction items from their inventory for quest-point bids.       *
 * Auctions run for AUCTION_DURATION ticks.  At expiry:                   *
 *   - If bids exist: winner receives item; seller receives QPs.          *
 *   - If no bids:    seller's item is returned on next login.            *
 *                                                                         *
 * Commands (do_auction):                                                  *
 *   auction list              — list all active lots                     *
 *   auction info <lot>        — full details on a lot                    *
 *   auction sell <item> <min> — put an item up with a minimum bid        *
 *   auction bid <lot> <amt>   — place a bid                              *
 *   auction withdraw <lot>    — cancel your own auction (no bids only)   *
 *                                                                         *
 * Persistence: auction.dat holds active auctions.                        *
 * Delivery:    auction_deliver() is called when a character enters the   *
 *              game; it hands over any lots they won while offline.      *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

/* =========================================================================
 * Globals
 * ========================================================================= */
static AUCTION_DATA *auction_list     = NULL;
static int           auction_next_lot = 1;

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

static AUCTION_DATA *new_auction( void )
{
    AUCTION_DATA *pAuc = calloc(1, sizeof(AUCTION_DATA));
    pAuc->next = NULL;
    pAuc->obj  = NULL;
    pAuc->seller[0] = '\0';
    pAuc->bidder[0] = '\0';
    pAuc->min_bid  = AUCTION_MIN_BID;
    pAuc->cur_bid  = 0;
    pAuc->timer    = AUCTION_DURATION;
    pAuc->lot      = auction_next_lot++;
    return pAuc;
}

/* Count active auctions. */
static int auction_count( void )
{
    AUCTION_DATA *p;
    int n = 0;
    for ( p = auction_list; p != NULL; p = p->next ) n++;
    return n;
}

/* Find lot by number. */
static AUCTION_DATA *find_lot( int lot )
{
    AUCTION_DATA *p;
    for ( p = auction_list; p != NULL; p = p->next )
        if ( p->lot == lot ) return p;
    return NULL;
}

/* Remove lot from list, free it (NOT the obj — caller handles that). */
static void remove_lot( AUCTION_DATA *pAuc )
{
    AUCTION_DATA *prev = NULL, *p;
    for ( p = auction_list; p != NULL; p = p->next )
    {
        if ( p == pAuc )
        {
            if ( prev ) prev->next = p->next;
            else        auction_list = p->next;
            free(p);
            return;
        }
        prev = p;
    }
}

/* Broadcast to all connected players. */
static void auction_broadcast( const char *msg )
{
    DESCRIPTOR_DATA *d;
    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING && d->character )
            send_to_char(msg, d->character);
}

/* Try to deliver an item to an online player, return TRUE on success. */
static bool deliver_to_online( const char *name, OBJ_DATA *obj )
{
    DESCRIPTOR_DATA *d;
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected != CON_PLAYING ) continue;
        if ( !d->character || IS_NPC(d->character) ) continue;
        if ( str_cmp(d->character->name, name) != 0 ) continue;

        obj_to_char(obj, d->character);
        send_to_char("\n\r#Y[AUCTION] #GYour won auction item has been placed in your inventory!#n\n\r",
                     d->character);
        return TRUE;
    }
    return FALSE;
}

/* =========================================================================
 * auction_update
 * Called every PULSE_TICK from update_handler.
 * ========================================================================= */
void auction_update( void )
{
    AUCTION_DATA *pAuc, *pAuc_next;
    char          buf[MAX_STRING_LENGTH];

    for ( pAuc = auction_list; pAuc != NULL; pAuc = pAuc_next )
    {
        pAuc_next = pAuc->next;
        pAuc->timer--;

        /* Announce at 5 and 1 minute remaining */
        if ( pAuc->timer == 5 || pAuc->timer == 1 )
        {
            if ( pAuc->bidder[0] != '\0' )
                sprintf(buf,
                    "#Y[AUCTION] #7Lot #W%d#7 (%s) — #W%d#7 minute(s) left, "
                    "current bid #R%d QP#7 by %s.\n\r",
                    pAuc->lot, pAuc->obj->short_descr,
                    pAuc->timer, pAuc->cur_bid, pAuc->bidder);
            else
                sprintf(buf,
                    "#Y[AUCTION] #7Lot #W%d#7 (%s) — #W%d#7 minute(s) left, "
                    "#Wno bids yet#7 (min #R%d QP#7).\n\r",
                    pAuc->lot, pAuc->obj->short_descr,
                    pAuc->timer, pAuc->min_bid);
            auction_broadcast(buf);
        }

        if ( pAuc->timer > 0 ) continue;

        /* --- Auction expired --- */
        if ( pAuc->bidder[0] == '\0' )
        {
            /* No bids: return item to seller */
            sprintf(buf,
                "#Y[AUCTION] #7Lot #W%d#7 (%s) expired with no bids. "
                "Returning to seller %s.\n\r",
                pAuc->lot, pAuc->obj->short_descr, pAuc->seller);
            auction_broadcast(buf);

            if ( !deliver_to_online(pAuc->seller, pAuc->obj) )
            {
                /* Save for next login */
                auction_save();
            }
        }
        else
        {
            /* Bids exist: pay seller, deliver to winner */
            int qp = pAuc->cur_bid;

            /* Pay the seller (if online) */
            DESCRIPTOR_DATA *d;
            for ( d = descriptor_list; d != NULL; d = d->next )
            {
                if ( d->connected != CON_PLAYING || !d->character ) continue;
                if ( str_cmp(d->character->name, pAuc->seller) != 0 ) continue;
                d->character->pcdata->quest += qp;
                sprintf(buf,
                    "#Y[AUCTION] #GYour lot %d (%s) sold to %s for #R%d QP#G!#n\n\r",
                    pAuc->lot, pAuc->obj->short_descr, pAuc->bidder, qp);
                send_to_char(buf, d->character);
                save_char_obj(d->character);
                break;
            }

            sprintf(buf,
                "#Y[AUCTION] #7Lot #W%d#7 (%s) sold to #W%s#7 for #R%d QP#7!\n\r",
                pAuc->lot, pAuc->obj->short_descr, pAuc->bidder, qp);
            auction_broadcast(buf);

            /* Deliver item to winner */
            if ( !deliver_to_online(pAuc->bidder, pAuc->obj) )
            {
                /* Winner is offline — save pending delivery */
                auction_save();
            }
        }

        remove_lot(pAuc);
    }
}

/* =========================================================================
 * do_auction
 * ========================================================================= */
void do_auction( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if ( IS_NPC(ch) ) return;

    /* ---- LIST ---- */
    if ( arg1[0] == '\0' || !str_cmp(arg1, "list") )
    {
        if ( auction_list == NULL )
        {
            send_to_char("#Y[AUCTION] #7No items are currently on auction.\n\r", ch);
            return;
        }
        AUCTION_DATA *p;
        send_to_char(
            "#Y+--[Lot]--[Item]-------------------[Min Bid]--[Cur Bid]--[Bidder]----[Time]+#n\n\r", ch);
        for ( p = auction_list; p != NULL; p = p->next )
        {
            sprintf(buf,
                "#Y| #W%-4d#7  %-24.24s  #R%6d QP  %6d QP#7  %-10.10s  %3d min #Y|#n\n\r",
                p->lot,
                p->obj ? p->obj->short_descr : "(unknown)",
                p->min_bid, p->cur_bid,
                p->bidder[0] ? p->bidder : "none",
                p->timer);
            send_to_char(buf, ch);
        }
        send_to_char(
            "#Y+-----------------------------------------------------------------------+#n\n\r", ch);
        return;
    }

    /* ---- INFO ---- */
    if ( !str_cmp(arg1, "info") )
    {
        if ( !is_number(arg2) ) { send_to_char("Syntax: auction info <lot>\n\r", ch); return; }
        AUCTION_DATA *p = find_lot(atoi(arg2));
        if ( !p ) { send_to_char("That lot does not exist.\n\r", ch); return; }
        sprintf(buf,
            "#Y[AUCTION] Lot #W%d#n\n\r"
            "  Item    : #W%s#n\n\r"
            "  Seller  : %s\n\r"
            "  Min bid : #R%d QP#n\n\r"
            "  Cur bid : #R%d QP#n  by %s\n\r"
            "  Time    : %d minute(s) remaining\n\r",
            p->lot,
            p->obj ? p->obj->short_descr : "(unknown)",
            p->seller,
            p->min_bid,
            p->cur_bid,
            p->bidder[0] ? p->bidder : "nobody",
            p->timer);
        send_to_char(buf, ch);
        return;
    }

    /* ---- SELL ---- */
    if ( !str_cmp(arg1, "sell") )
    {
        OBJ_DATA *obj;
        int        min_bid;

        if ( arg2[0] == '\0' )
        {
            send_to_char("Syntax: auction sell <item> <minimum bid>\n\r", ch);
            return;
        }

        /* Parse: arg2 is item keyword, argument (remaining) is min bid */
        obj = get_obj_carry(ch, arg2);
        if ( !obj )
        {
            send_to_char("You don't seem to have that item.\n\r", ch);
            return;
        }

        if ( argument[0] == '\0' || !is_number(argument) )
        {
            send_to_char("Syntax: auction sell <item> <minimum bid in QP>\n\r", ch);
            return;
        }
        min_bid = atoi(argument);

        if ( min_bid < AUCTION_MIN_BID )
        {
            sprintf(buf, "Minimum opening bid is %d QP.\n\r", AUCTION_MIN_BID);
            send_to_char(buf, ch);
            return;
        }

        if ( IS_SET(obj->extra_flags, ITEM_NODROP) )
        {
            send_to_char("You cannot auction a no-drop item.\n\r", ch);
            return;
        }

        if ( auction_count() >= AUCTION_MAX )
        {
            send_to_char("The auction house is full. Try again later.\n\r", ch);
            return;
        }

        /* Create the lot */
        AUCTION_DATA *pAuc = new_auction();
        strncpy(pAuc->seller, ch->name, sizeof(pAuc->seller) - 1);
        pAuc->min_bid = min_bid;

        obj_from_char(obj);
        pAuc->obj = obj;

        /* Link into list */
        pAuc->next   = auction_list;
        auction_list = pAuc;

        sprintf(buf,
            "#Y[AUCTION] #7%s puts #W%s#7 up for auction "
            "(lot #W%d#7, min #R%d QP#7, %d min).\n\r",
            ch->name, obj->short_descr,
            pAuc->lot, pAuc->min_bid, pAuc->timer);
        auction_broadcast(buf);
        auction_save();
        return;
    }

    /* ---- BID ---- */
    if ( !str_cmp(arg1, "bid") )
    {
        AUCTION_DATA *pAuc;
        int           lot, amount;

        if ( !is_number(arg2) || argument[0] == '\0' || !is_number(argument) )
        {
            send_to_char("Syntax: auction bid <lot> <amount>\n\r", ch);
            return;
        }
        lot    = atoi(arg2);
        amount = atoi(argument);
        pAuc   = find_lot(lot);

        if ( !pAuc )
        {
            send_to_char("That lot does not exist.\n\r", ch);
            return;
        }
        if ( !str_cmp(pAuc->seller, ch->name) )
        {
            send_to_char("You cannot bid on your own auction.\n\r", ch);
            return;
        }
        if ( amount < pAuc->min_bid )
        {
            sprintf(buf, "Minimum bid for this lot is %d QP.\n\r", pAuc->min_bid);
            send_to_char(buf, ch);
            return;
        }
        if ( amount <= pAuc->cur_bid )
        {
            sprintf(buf, "Current bid is %d QP — you must bid at least %d QP.\n\r",
                    pAuc->cur_bid, pAuc->cur_bid + AUCTION_BID_STEP);
            send_to_char(buf, ch);
            return;
        }
        if ( ch->pcdata->quest < amount )
        {
            send_to_char("You don't have that many quest points.\n\r", ch);
            return;
        }

        /* Refund the previous bidder if online */
        if ( pAuc->bidder[0] != '\0' )
        {
            DESCRIPTOR_DATA *d;
            for ( d = descriptor_list; d != NULL; d = d->next )
            {
                if ( d->connected != CON_PLAYING || !d->character ) continue;
                if ( str_cmp(d->character->name, pAuc->bidder) != 0 ) continue;
                d->character->pcdata->quest += pAuc->cur_bid;
                sprintf(buf,
                    "#Y[AUCTION] #7You have been outbid on lot %d (%s). "
                    "#R%d QP#7 refunded.\n\r",
                    pAuc->lot, pAuc->obj->short_descr, pAuc->cur_bid);
                send_to_char(buf, d->character);
                break;
            }
        }

        ch->pcdata->quest -= amount;
        pAuc->cur_bid = amount;
        strncpy(pAuc->bidder, ch->name, sizeof(pAuc->bidder) - 1);

        sprintf(buf,
            "#Y[AUCTION] #7%s bids #R%d QP#7 on lot #W%d#7 (%s).\n\r",
            ch->name, amount, pAuc->lot, pAuc->obj->short_descr);
        auction_broadcast(buf);
        save_char_obj(ch);
        auction_save();
        return;
    }

    /* ---- WITHDRAW ---- */
    if ( !str_cmp(arg1, "withdraw") )
    {
        AUCTION_DATA *pAuc;
        int           lot;

        if ( !is_number(arg2) )
        {
            send_to_char("Syntax: auction withdraw <lot>\n\r", ch);
            return;
        }
        lot  = atoi(arg2);
        pAuc = find_lot(lot);

        if ( !pAuc )
        {
            send_to_char("That lot does not exist.\n\r", ch);
            return;
        }
        if ( str_cmp(pAuc->seller, ch->name) )
        {
            send_to_char("You can only withdraw your own auctions.\n\r", ch);
            return;
        }
        if ( pAuc->bidder[0] != '\0' )
        {
            send_to_char("You cannot withdraw a lot that already has bids.\n\r", ch);
            return;
        }

        obj_to_char(pAuc->obj, ch);
        pAuc->obj = NULL;

        sprintf(buf,
            "#Y[AUCTION] #7%s withdraws lot #W%d#7 from auction.\n\r",
            ch->name, pAuc->lot);
        auction_broadcast(buf);
        remove_lot(pAuc);
        auction_save();
        return;
    }

    send_to_char(
        "Syntax: auction list\n\r"
        "        auction info <lot>\n\r"
        "        auction sell <item> <min bid>\n\r"
        "        auction bid <lot> <amount>\n\r"
        "        auction withdraw <lot>\n\r", ch);
}

/* =========================================================================
 * auction_deliver
 * Called in load_char_obj when a player enters the game.
 * Delivers any lots they won while offline.
 * ========================================================================= */
void auction_deliver( CHAR_DATA *ch )
{
    AUCTION_DATA *p, *p_next;
    char          buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) return;

    for ( p = auction_list; p != NULL; p = p_next )
    {
        p_next = p->next;

        /* Items in auction_list with timer <= 0 and a bidder == ch are pending */
        if ( p->timer > 0 ) continue;
        if ( str_cmp(p->bidder, ch->name) != 0 ) continue;
        if ( !p->obj ) continue;

        obj_to_char(p->obj, ch);
        p->obj = NULL;

        sprintf(buf,
            "#Y[AUCTION] #GYou receive lot %d (%s) — the auction you won!#n\n\r",
            p->lot, p->obj ? p->obj->short_descr : "item");
        send_to_char(buf, ch);

        remove_lot(p);
    }

    auction_save();
}

/* =========================================================================
 * auction_save / auction_load
 * Simple flat-file persistence using fwrite_obj/fread_obj conventions.
 * ========================================================================= */

void auction_save( void )
{
    AUCTION_DATA *p;
    FILE         *fp;

    fp = fopen(AUCTION_FILE, "w");
    if ( !fp )
    {
        mud_logf("auction_save: cannot open %s for writing.", AUCTION_FILE);
        return;
    }

    fprintf(fp, "#AUCTION_NEXT_LOT\n%d\n", auction_next_lot);

    for ( p = auction_list; p != NULL; p = p->next )
    {
        if ( !p->obj ) continue;

        fprintf(fp,
            "#AUCTION\n"
            "Lot       %d\n"
            "Seller    %s~\n"
            "Bidder    %s~\n"
            "MinBid    %d\n"
            "CurBid    %d\n"
            "Timer     %d\n",
            p->lot,
            p->seller,
            p->bidder[0] ? p->bidder : "",
            p->min_bid,
            p->cur_bid,
            p->timer);

        /* Write item using the same fields as save.c fwrite_obj */
        OBJ_DATA *obj = p->obj;
        fprintf(fp,
            "ObjVnum   %d\n"
            "ObjName   %s~\n"
            "ObjShort  %s~\n"
            "ObjLong   %s~\n"
            "ObjExtra  %d\n"
            "ObjWear   %d\n"
            "ObjVal    %d %d %d %d\n"
            "#END\n",
            obj->pIndexData->vnum,
            obj->name,
            obj->short_descr,
            obj->description,
            obj->extra_flags,
            obj->wear_flags,
            obj->value[0], obj->value[1], obj->value[2], obj->value[3]);
    }

    fprintf(fp, "#END_AUCTIONS\n");
    fclose(fp);
}

void auction_load( void )
{
    FILE *fp;
    char  word[256];
    int   lot = 0, min_bid = 0, cur_bid = 0, timer_val = 0;
    char  seller[50] = {'\0'}, bidder[50] = {'\0'};
    int   obj_vnum = 0;
    bool  in_auction = FALSE;

    fp = fopen(AUCTION_FILE, "r");
    if ( !fp )
    {
        log_string("auction_load: no auction file found, starting fresh.");
        return;
    }

    while ( fscanf(fp, "%255s", word) == 1 )
    {
        if ( !str_cmp(word, "#AUCTION_NEXT_LOT") )
        {
            fscanf(fp, "%d", &auction_next_lot);
        }
        else if ( !str_cmp(word, "#AUCTION") )
        {
            in_auction = TRUE;
            lot = 0; min_bid = 0; cur_bid = 0; timer_val = 0;
            seller[0] = '\0'; bidder[0] = '\0'; obj_vnum = 0;
        }
        else if ( in_auction )
        {
            if ( !str_cmp(word, "Lot"    ) ) { fscanf(fp, "%d", &lot);      }
            else if ( !str_cmp(word, "MinBid" ) ) { fscanf(fp, "%d", &min_bid); }
            else if ( !str_cmp(word, "CurBid" ) ) { fscanf(fp, "%d", &cur_bid); }
            else if ( !str_cmp(word, "Timer"  ) ) { fscanf(fp, "%d", &timer_val); }
            else if ( !str_cmp(word, "Seller" ) ) { fscanf(fp, "%49s", seller); fread_to_eol(fp); }
            else if ( !str_cmp(word, "Bidder" ) ) { fscanf(fp, "%49s", bidder); fread_to_eol(fp); }
            else if ( !str_cmp(word, "ObjVnum") ) { fscanf(fp, "%d", &obj_vnum); }
            else if ( !str_cmp(word, "#END"   ) )
            {
                if ( obj_vnum > 0 )
                {
                    OBJ_INDEX_DATA *pIdx = get_obj_index(obj_vnum);
                    if ( pIdx )
                    {
                        AUCTION_DATA *pAuc = new_auction();
                        pAuc->lot     = lot;
                        pAuc->min_bid = min_bid;
                        pAuc->cur_bid = cur_bid;
                        pAuc->timer   = timer_val;
                        strncpy(pAuc->seller, seller, sizeof(pAuc->seller)-1);
                        strncpy(pAuc->bidder, bidder, sizeof(pAuc->bidder)-1);
                        pAuc->obj     = create_object(pIdx, 0);
                        pAuc->next    = auction_list;
                        auction_list  = pAuc;
                    }
                }
                in_auction = FALSE;
            }
        }
        else if ( !str_cmp(word, "#END_AUCTIONS") ) break;
    }

    fclose(fp);
    mud_logf("auction_load: auction house loaded.");
}
