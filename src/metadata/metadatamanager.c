//
// Created by Sam on 2018/1/26.
//

#include "metadatamanager.h"

int metadata_manager_create(metadata_manager *metadataManager, const char *file, transaction *tx, int isNew){
    metadataManager->tableManager =    table_manager_create( isNew, tx);
};

table_manager *table_manager_create(int isNew, transaction *tx) {
    table_manager *tableManager = (table_manager *)malloc(sizeof(table_manager));
    hmap_t tableDescfields = hashmap_create();

    field_info *fieldInfo = (field_info *)malloc(sizeof(field_info));
    field_info_create(fieldInfo, DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(tableDescfields, "tablename", fieldInfo);

    fieldInfo = (field_info *)malloc(sizeof(field_info));
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(tableDescfields, "reclength", fieldInfo);

    array_list fieldsName = array_list_create(MAX_ID_NAME_LENGTH);
    array_list_add(&fieldsName, "tablename");
    array_list_add(&fieldsName, "reclength");

    tableManager->tcatInfo = (table_info *)malloc(sizeof(table_info));
    table_info_create(tableManager->tcatInfo, "tablecat", fieldsName, tableDescfields);

    if (isNew){
        table_manager_create_table(tableManager, "tablecat", fieldsName, tableDescfields, tx);
    }

    hmap_t fieldDescfields = hashmap_create();
    field_info_create(fieldInfo, DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(fieldDescfields, "tablename", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_CHAR, MAX_ID_NAME_LENGTH);
    hashmap_put(fieldDescfields, "fieldname", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "type", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "length", fieldInfo);
    field_info_create(fieldInfo, DATA_TYPE_INT, INT_SIZE);
    hashmap_put(fieldDescfields, "offset", fieldInfo);

    fieldsName = array_list_create(MAX_ID_NAME_LENGTH);
    array_list_add(&fieldsName, "tablename");
    array_list_add(&fieldsName, "fieldname");
    array_list_add(&fieldsName, "type");
    array_list_add(&fieldsName, "length");
    array_list_add(&fieldsName, "offset");

    tableManager->fcatInfo = (table_info *)malloc(sizeof(table_info));
    table_info_create(tableManager->fcatInfo, "fieldcat", fieldsName, fieldDescfields);

    if(isNew){
        table_manager_create_table(tableManager, "fieldcat", fieldsName, fieldDescfields, tx);
    }
    return tableManager;
};

/**
 * 创建tableInfo，创建表
 * @param tableManager
 * @param tableName
 * @param fieldsName
 * @param fields
 * @param tx
 * @return
 */
int table_manager_create_table(table_manager *tableManager, char *tableName, array_list fieldsName, hmap_t fields, transaction *tx) {
    table_info *tableInfo = (table_info *)malloc(sizeof(table_info));
    table_info_create(tableInfo, tableName, fieldsName, fields);

    /*打开元数据表*/
    record_file *tcatFile = (record_file *) malloc(sizeof(record_file));
    record_file_create(tcatFile, tableManager->tcatInfo, tx);

    /*增加元数据到table描述表中*/
    record_file_insert(tcatFile);
    record_file_set_string(tcatFile, "tablename", tableName);
    record_file_set_int(tcatFile, "reclength", tableInfo->recordLen);
    record_file_close(tcatFile);
/*增加元数据到field描述表中*/
};

int table_manager_get_tableinfo(table_manager *tableManager, char *tableName, transaction tx, hmap_t fields) {

};