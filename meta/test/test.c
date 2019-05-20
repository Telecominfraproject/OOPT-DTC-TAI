#include "tai.h"
#include "taimetadata.h"
#include <stdio.h>
#include <string.h>

int testSerializeModuleOperStatus() {
    int ret;
    char buf[128] = {0};
    ret = tai_serialize_module_oper_status(buf, 128, TAI_MODULE_OPER_STATUS_READY, NULL);
    if ( ret < 0 ) {
        return ret;
    }
    ret = strcmp(buf, "TAI_MODULE_OPER_STATUS_READY");
    if ( ret != 0 ) {
        return -1;
    }
    return 0;
}

int testDeserializeModuleOperStatus() {
    int ret, value;
    ret = tai_deserialize_module_oper_status("TAI_MODULE_OPER_STATUS_INITIALIZE", &value, NULL);
    if ( ret < 0 ) {
        return ret;
    }
    if ( TAI_MODULE_OPER_STATUS_INITIALIZE != value ) {
        return -1;
    }
    return 0;
}

int testSerializeAttributeEnum() {
    int ret;
    char buf[128] = {0};
    tai_attribute_t attr = {0};
    tai_serialize_option_t option = {
        .valueonly = true,
    };
    const tai_attr_metadata_t* meta = tai_metadata_get_attr_metadata(TAI_OBJECT_TYPE_MODULE, TAI_MODULE_ATTR_OPER_STATUS);
    attr.id = TAI_MODULE_ATTR_OPER_STATUS;
    attr.value.s32 = TAI_MODULE_OPER_STATUS_READY;
    ret = tai_serialize_attribute(buf, 128, meta, &attr, &option);
    if ( ret <  0 ) {
        return -1;
    }
    ret = strcmp(buf, "TAI_MODULE_OPER_STATUS_READY");
    if ( ret != 0 ) {
        return -1;
    }
    return 0;
}

int testSerializeAttributeFloat() {
    int ret;
    char buf[128] = {0};
    tai_attribute_t attr = {0};
    tai_serialize_option_t option = {
        .valueonly = true,
    };
    const tai_attr_metadata_t* meta = tai_metadata_get_attr_metadata(TAI_OBJECT_TYPE_MODULE, TAI_MODULE_ATTR_TEMP);
    attr.id = TAI_MODULE_ATTR_TEMP;
    attr.value.flt = 1.10;
    ret = tai_serialize_attribute(buf, 128, meta, &attr, &option);
    if ( ret < 0 ) {
        return -1;
    }
    ret = strcmp(buf, "1.100000");
    if ( ret != 0 ) {
        return -1;
    }
    return 0;
}

int testSerializeAttributeEnumList() {
    int ret;
    char buf[128] = {0};
    tai_attribute_t attr = {0};
    tai_serialize_option_t option = {
        .valueonly = false,
        .human = true,
    };
    const tai_attr_metadata_t* meta = tai_metadata_get_attr_metadata(TAI_OBJECT_TYPE_NETWORKIF, TAI_NETWORK_INTERFACE_ATTR_TX_ALIGN_STATUS);
    int32_t list[] = {
        TAI_NETWORK_INTERFACE_TX_ALIGN_STATUS_LOSS,
        TAI_NETWORK_INTERFACE_TX_ALIGN_STATUS_OUT
    };
    attr.id = TAI_NETWORK_INTERFACE_ATTR_TX_ALIGN_STATUS;
    attr.value.s32list.count = 2;
    attr.value.s32list.list = (int32_t*)&list;
    ret = tai_serialize_attribute(buf, 128, meta, &attr, &option);
    if ( ret < 0 ) {
        return -1;
    }
    ret = strcmp(buf, "tx-align-status | loss|out");
    if ( ret != 0 ) {
        return -1;
    }
    return 0;
}

int testGetAttrMetadataByAttrIdName() {
    const tai_attr_metadata_t* meta;
    meta = tai_metadata_get_attr_metadata_by_attr_id_name("TAI_NETWORK_INTERFACE_ATTR_TX_LASER_FREQ");
    if ( meta == NULL ) {
        return -1;
    }
    if ( meta->attrid != TAI_NETWORK_INTERFACE_ATTR_TX_LASER_FREQ ) {
        return -1;
    }
 
    return 0;
}

int testDeserializeNetworkInterfaceAttr() {
    int32_t value;
    int ret;
    tai_serialize_option_t option = {
        .human = true,
    };
    ret = tai_deserialize_network_interface_attr("tx-laser-freq", &value, &option);
    if ( ret < 0 ) {
        return -1;
    }
    if ( value != TAI_NETWORK_INTERFACE_ATTR_TX_LASER_FREQ ) {
        return -1;
    }
    return 0;
}

int testDeepcopyAttrValue() {
    const tai_attr_metadata_t* meta = tai_metadata_get_attr_metadata(TAI_OBJECT_TYPE_NETWORKIF, TAI_NETWORK_INTERFACE_ATTR_TX_ALIGN_STATUS);
    tai_attribute_t src, dst = {0};
    tai_status_t status;
    status = tai_metadata_alloc_attr_value(meta, &src, NULL);
    if ( status != TAI_STATUS_SUCCESS ) {
        printf("failed to alloc attr value: %d\n", status);
        return -1;
    }
    status = tai_metadata_alloc_attr_value(meta, &dst, NULL);
    if ( status != TAI_STATUS_SUCCESS ) {
        printf("failed to alloc attr value: %d\n", status);
        return -1;
    }
    src.value.s32list.count = 2;
    src.value.s32list.list[0] = TAI_NETWORK_INTERFACE_TX_ALIGN_STATUS_TIMING;
    src.value.s32list.list[1] = TAI_NETWORK_INTERFACE_TX_ALIGN_STATUS_OUT;

    status = tai_metadata_deepcopy_attr_value(meta, &src, &dst);
    if ( status != TAI_STATUS_SUCCESS ) {
        return -1;
    }

    if ( dst.value.s32list.count != 2 ) {
        return -1;
    }
    if ( dst.value.s32list.list[0] != TAI_NETWORK_INTERFACE_TX_ALIGN_STATUS_TIMING ) {
        return -1;
    }
    if ( dst.value.s32list.list[1] != TAI_NETWORK_INTERFACE_TX_ALIGN_STATUS_OUT ) {
        return -1;
    }

    status = tai_metadata_free_attr_value(meta, &src, NULL);
    if ( status != TAI_STATUS_SUCCESS ) {
        printf("failed to free attr value: %d\n", status);
        return -1;
    }
    status = tai_metadata_free_attr_value(meta, &dst, NULL);
    if ( status != TAI_STATUS_SUCCESS ) {
        printf("failed to free attr value: %d\n", status);
        return -1;
    }
    if ( dst.value.s32list.count != 0 ) {
        return -1;
    }
    if ( src.value.s32list.count != 0 ) {
        return -1;
    }
    return 0;
}

int testSerializeUnsignedRange() {
    char buf[128] = {0};
    tai_attribute_value_t value = {0};
    tai_attr_metadata_t meta = { .attrvaluetype = TAI_ATTR_VALUE_TYPE_U32RANGE };
    tai_serialize_option_t option = {
        .valueonly = true,
        .human = true,
    };
    int ret = tai_deserialize_attribute_value("100,1000", &meta, &value, NULL);
    tai_attribute_t attr = { .value = value };
    tai_serialize_attribute(buf, 128, &meta, &attr, &option);
    ret = strcmp(buf, "100,1000");
    if ( ret != 0 ) {
        return -1;
    }
    return 0;
}

int testSerializeSignedRange() {
    char buf[128] = {0};
    tai_attribute_value_t value = {0};
    tai_attr_metadata_t meta = { .attrvaluetype = TAI_ATTR_VALUE_TYPE_S32RANGE };
    tai_serialize_option_t option = {
        .valueonly = true,
        .human = true,
    };
    int ret = tai_deserialize_attribute_value("-100,-1000", &meta, &value, NULL);
    tai_attribute_t attr = { .value = value };
    tai_serialize_attribute(buf, 128, &meta, &attr, &option);
    ret = strcmp(buf, "-100,-1000");
    if ( ret != 0 ) {
        return -1;
    }
    return 0;
}

int testSerializeValueAttrList() {
    char buf[128] = {0};
    const tai_attr_metadata_t* meta;
    tai_attribute_t attr = {0};
    int ret;
    int32_t value[] = {
        TAI_HOST_INTERFACE_LANE_FAULT_LOSS_OF_LOCK,
        TAI_HOST_INTERFACE_LANE_FAULT_TX_FIFO_ERR,
    };
    tai_serialize_option_t option = {
        .human = true,
    };
    tai_attribute_value_t lane_fault;
    lane_fault.s32list.count = 2;
    lane_fault.s32list.list = value;
    tai_attribute_value_t list[] = {
        lane_fault,
        lane_fault,
    };
    meta = tai_metadata_get_attr_metadata_by_attr_id_name("TAI_HOST_INTERFACE_ATTR_LANE_FAULT");
    if ( meta == NULL ) {
        return -1;
    }
    attr.id = TAI_HOST_INTERFACE_ATTR_LANE_FAULT;
    attr.value.attrlist.count = 2;
    attr.value.attrlist.list = (tai_attribute_value_t*)&list;
    ret = tai_serialize_attribute(buf, 128, meta, &attr, &option);
    if ( ret < 0 ) {
        return -1;
    }
    ret = strcmp(buf, "lane-fault | loss-of-lock|tx-fifo-err, loss-of-lock|tx-fifo-err");
    if ( ret != 0 ) {
        return -1;
    }
    return 0;
}

int testDeserializeU8list() {
    uint8_t list[10] = {0};
    tai_u8_list_t value;
    value.count = 10;
    value.list = list;
    int ret = tai_deserialize_u8list("1,2,3,4", &value);
    if ( ret != 0 ) {
        return -1;
    }
    if ( value.count != 4 || list[0] != 1 || list[1] != 2 || list[2] != 3 || list[3] != 4) {
        return -1;
    }
    return 0;
}

int testDeserializeFloatlist() {
    float list[10] = {0};
    tai_float_list_t value;
    value.count = 10;
    value.list = list;
    int ret = tai_deserialize_floatlist("1.1,2.1234,3.1,4.5634", &value);
    if ( ret != 0 ) {
        return -1;
    }
    if ( value.count != 4 || list[0] < 1 || list[1] < 2.1 || list[2] > 3.2 || list[3] > 4.6 ) {
        return -1;
    }
    return 0;
}

int testSerializeListJSON() {
    char buf[128] = {0};
    const tai_attr_metadata_t* meta;
    tai_attribute_t attr = {0};
    int ret;
    int32_t value[] = {
        TAI_HOST_INTERFACE_LANE_FAULT_LOSS_OF_LOCK,
        TAI_HOST_INTERFACE_LANE_FAULT_TX_FIFO_ERR,
    };
    tai_serialize_option_t option = {
        .human = true,
        .json = true,
    };
    tai_attribute_value_t lane_fault;
    lane_fault.s32list.count = 2;
    lane_fault.s32list.list = value;
    tai_attribute_value_t list[] = {
        lane_fault,
        lane_fault,
    };
    meta = tai_metadata_get_attr_metadata_by_attr_id_name("TAI_HOST_INTERFACE_ATTR_LANE_FAULT");
    if ( meta == NULL ) {
        return -1;
    }
    attr.id = TAI_HOST_INTERFACE_ATTR_LANE_FAULT;
    attr.value.attrlist.count = 2;
    attr.value.attrlist.list = (tai_attribute_value_t*)&list;
    ret = tai_serialize_attribute(buf, 128, meta, &attr, &option);
    if ( ret < 0 ) {
        return -1;
    }
    return 0;
}

typedef int (*testF)();

testF tests[] = {
    testSerializeModuleOperStatus,
    testDeserializeModuleOperStatus,
    testSerializeAttributeEnum,
    testSerializeAttributeFloat,
    testSerializeAttributeEnumList,
    testGetAttrMetadataByAttrIdName,
    testDeserializeNetworkInterfaceAttr,
    testDeepcopyAttrValue,
    testSerializeUnsignedRange,
    testSerializeSignedRange,
    testSerializeValueAttrList,
    testDeserializeU8list,
    testDeserializeFloatlist,
    testSerializeListJSON,
    NULL,
};

int main() {
    int i = 0, ret;

    tai_metadata_log_level = TAI_LOG_LEVEL_DEBUG;

    while (true) {
        if( tests[i] == NULL ) {
            break;
        }
        ret = tests[i++]();
        if ( ret < 0 ) {
            printf("test %d failed\n", --i);
            return -ret;
        }
    }
    return 0;
}
