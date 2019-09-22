//
// Created by Sam on 2018/2/13.
//

#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>

/**
 * 在现有实现基础上，实现update from子句
 *
 * 支持的update语法：
 *
 * UPDATE <table_name> SET <field1> = <expr1>[, <field2 = <expr2>, ..]
 * WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_update 结构
 */

/*TODO: parse_sql_stmt_update， update语句解析*/
sql_stmt_update *UpdateParser::parse_sql_stmt_update()
{
//  fprintf(stderr, "TODO: update is not implemented yet. in parse_sql_stmt_update \n");
  sql_stmt_update *sqlStmtUpdate = (sql_stmt_update*)malloc(sizeof(sql_stmt_update));
  char *tableName = nullptr;
  vector<char*> fields;
  vector<Expression*> fieldsExpr;
  SRA_t *where = nullptr;
};