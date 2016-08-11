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
::
:: include('_copyright.c')

/**
 * Test extensions
 */

#include <locitest/test_common.h>

/**
 * Simple tests for extension objects
 */

int
test_ext_objs(void)
{
    of_action_bsn_mirror_t *obj;

    obj = of_action_bsn_mirror_new(OF_VERSION_1_0);
    TEST_ASSERT(obj != NULL);
    TEST_ASSERT(obj->object_id == OF_ACTION_BSN_MIRROR);

    TEST_ASSERT(of_action_to_object_id(OF_EXPERIMENTER_TYPE, OF_VERSION_1_0) ==
                OF_ACTION_EXPERIMENTER);

    TEST_ASSERT(of_action_id_to_object_id(OF_EXPERIMENTER_TYPE, OF_VERSION_1_0) ==
                OF_ACTION_ID_EXPERIMENTER);

    TEST_ASSERT(of_instruction_to_object_id(OF_EXPERIMENTER_TYPE, OF_VERSION_1_0) ==
                OF_INSTRUCTION_EXPERIMENTER);

    TEST_ASSERT(of_queue_prop_to_object_id(OF_EXPERIMENTER_TYPE, OF_VERSION_1_0) ==
                OF_QUEUE_PROP_EXPERIMENTER);

    TEST_ASSERT(of_meter_band_to_object_id(OF_EXPERIMENTER_TYPE, OF_VERSION_1_0) ==
                OF_METER_BAND_EXPERIMENTER);

    TEST_ASSERT(of_table_feature_prop_to_object_id(OF_EXPERIMENTER_TYPE-1,
                                                   OF_VERSION_1_3) ==
                OF_TABLE_FEATURE_PROP_EXPERIMENTER);

    TEST_ASSERT(of_table_feature_prop_to_object_id(OF_EXPERIMENTER_TYPE,
                                                   OF_VERSION_1_3) ==
                OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS);

    return TEST_PASS;
}