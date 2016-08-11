:: # Copyright 2013, Big Switch Networks, Inc.
:: #
:: # LoxiGen is licensed under the Eclipse Public License, version 1.0 (EPL), with
:: # the following special exception:
:: #
:: # LOXI Exception
:: #
:: # As a special exception to the terms of the EPL, you may distribute libraries
:: # generated by LoxiGen (LoxiGen Libraries) under the terms of your choice, provided
:: # that copyright and licensing notices generated by LoxiGen are not altered or removed
:: # from the LoxiGen Libraries and the notice provided below is (i) included in
:: # the LoxiGen Libraries, if distributed in source code form and (ii) included in any
:: # documentation for the LoxiGen Libraries, if distributed in binary form.
:: #
:: # Notice: "Copyright 2013, Big Switch Networks, Inc. This library was generated by the LoxiGen Compiler."
:: #
:: # You may not use this file except in compliance with the EPL or LOXI Exception. You may obtain
:: # a copy of the EPL at:
:: #
:: # http://www.eclipse.org/legal/epl-v10.html
:: #
:: # Unless required by applicable law or agreed to in writing, software
:: # distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
:: # WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
:: # EPL for the specific language governing permissions and limitations
:: # under the EPL.
:: include('_copyright.c')
#include <loci/loci.h>
#include <loci/of_object.h>
#include "loci_log.h"
#include "loci_int.h"

/* Flow stats entry setup for all versions */

static int
flow_stats_entry_setup_from_flow_add_common(of_flow_stats_entry_t *obj,
                                            of_flow_add_t *flow_add,
                                            of_object_t *effects,
                                            int entry_match_offset,
                                            int add_match_offset)
{
    int entry_len, add_len;
    of_wire_buffer_t *wbuf;
    int abs_offset;
    int delta;
    uint16_t val16;
    uint64_t cookie;
    of_octets_t match_octets;

    /* Transfer the match underlying object from add to stats entry */
    wbuf = OF_OBJECT_TO_WBUF(obj);
    entry_len = _WIRE_MATCH_PADDED_LEN(obj, entry_match_offset);
    add_len = _WIRE_MATCH_PADDED_LEN(flow_add, add_match_offset);

    match_octets.bytes = add_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(flow_add, add_match_offset);

    /* Copy data into flow entry */
    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, entry_match_offset);
    of_wire_buffer_replace_data(wbuf, abs_offset, entry_len,
                                match_octets.data, add_len);

    /* Not scalar, update lengths if needed */
    delta = add_len - entry_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    of_flow_add_cookie_get(flow_add, &cookie);
    of_flow_stats_entry_cookie_set(obj, cookie);

    of_flow_add_priority_get(flow_add, &val16);
    of_flow_stats_entry_priority_set(obj, val16);

    of_flow_add_idle_timeout_get(flow_add, &val16);
    of_flow_stats_entry_idle_timeout_set(obj, val16);

    of_flow_add_hard_timeout_get(flow_add, &val16);
    of_flow_stats_entry_hard_timeout_set(obj, val16);

    /* Effects may come from different places */
    if (effects != NULL) {
        if (obj->version == OF_VERSION_1_0) {
            OF_TRY(of_flow_stats_entry_actions_set(obj,
                (of_list_action_t *)effects));
        } else {
            OF_TRY(of_flow_stats_entry_instructions_set(obj,
                (of_list_instruction_t *)effects));
        }
    } else {
        if (obj->version == OF_VERSION_1_0) {
            of_list_action_t actions;
            of_flow_add_actions_bind(flow_add, &actions);
            OF_TRY(of_flow_stats_entry_actions_set(obj, &actions));
        } else {
            of_list_instruction_t instructions;
            of_flow_add_instructions_bind(flow_add, &instructions);
            OF_TRY(of_flow_stats_entry_instructions_set(obj, &instructions));
        }
    }

    return OF_ERROR_NONE;
}

/* Flow removed setup for all versions */

static int
flow_removed_setup_from_flow_add_common(of_flow_removed_t *obj,
                                        of_flow_add_t *flow_add,
                                        int removed_match_offset,
                                        int add_match_offset)
{
    int add_len, removed_len;
    of_wire_buffer_t *wbuf;
    int abs_offset;
    int delta;
    uint16_t val16;
    uint64_t cookie;
    of_octets_t match_octets;

    /* Transfer the match underlying object from add to removed obj */
    wbuf = OF_OBJECT_TO_WBUF(obj);
    removed_len = _WIRE_MATCH_PADDED_LEN(obj, removed_match_offset);
    add_len = _WIRE_MATCH_PADDED_LEN(flow_add, add_match_offset);

    match_octets.bytes = add_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(flow_add, add_match_offset);

    /* Copy data into flow removed */
    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, removed_match_offset);
    of_wire_buffer_replace_data(wbuf, abs_offset, removed_len,
                                match_octets.data, add_len);

    /* Not scalar, update lengths if needed */
    delta = add_len - removed_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    of_flow_add_cookie_get(flow_add, &cookie);
    of_flow_removed_cookie_set(obj, cookie);

    of_flow_add_priority_get(flow_add, &val16);
    of_flow_removed_priority_set(obj, val16);

    of_flow_add_idle_timeout_get(flow_add, &val16);
    of_flow_removed_idle_timeout_set(obj, val16);

    if (obj->version >= OF_VERSION_1_2) {
        of_flow_add_hard_timeout_get(flow_add, &val16);
        of_flow_removed_hard_timeout_set(obj, val16);
    }

    return OF_ERROR_NONE;
}

/* Set up a flow removed message from the original add */

int
of_flow_removed_setup_from_flow_add(of_flow_removed_t *obj,
                                    of_flow_add_t *flow_add)
{
    switch (obj->version) {
    case OF_VERSION_1_0:
        return flow_removed_setup_from_flow_add_common(obj, flow_add,
                                                       8, 8);
        break;
    default:
        return flow_removed_setup_from_flow_add_common(obj, flow_add,
                                                       48, 48);
        break;
    }

    return OF_ERROR_NONE;
}


/* Set up a packet in message from the original add */

int
of_packet_in_setup_from_flow_add(of_packet_in_t *obj,
                                 of_flow_add_t *flow_add)
{
    int add_len, pkt_in_len;
    of_wire_buffer_t *wbuf;
    int abs_offset;
    int delta;
    const int pkt_in_match_offset = 16;
    const int add_match_offset = 48;
    of_octets_t match_octets;

    if (obj->version < OF_VERSION_1_2) {
        /* Nothing to be done before OF 1.2 */
        return OF_ERROR_NONE;
    }

    /* Transfer match struct from flow add to packet in object */
    wbuf = OF_OBJECT_TO_WBUF(obj);
    pkt_in_len = _WIRE_MATCH_PADDED_LEN(obj, pkt_in_match_offset);
    add_len = _WIRE_MATCH_PADDED_LEN(flow_add, add_match_offset);

    match_octets.bytes = add_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(flow_add, add_match_offset);

    /* Copy data into pkt_in msg */
    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, pkt_in_match_offset);
    of_wire_buffer_replace_data(wbuf, abs_offset, pkt_in_len,
                                match_octets.data, add_len);

    /* Not scalar, update lengths if needed */
    delta = add_len - pkt_in_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    return OF_ERROR_NONE;
}

/* Set up a stats entry from the original add */

int
of_flow_stats_entry_setup_from_flow_add(of_flow_stats_entry_t *obj,
                                        of_flow_add_t *flow_add,
                                        of_object_t *effects)
{
    switch (obj->version) {
    case OF_VERSION_1_0:
        return flow_stats_entry_setup_from_flow_add_common(obj, flow_add,
                                                           effects, 4, 8);
        break;
    default:
        return flow_stats_entry_setup_from_flow_add_common(obj, flow_add,
                                                           effects, 48, 48);
        break;
    }

    return OF_ERROR_NONE;
}
