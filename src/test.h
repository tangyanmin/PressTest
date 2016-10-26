/****************************************************************************
 * Copyright (C), 2016, Sensetime Tech. Co., Ltd.
 * file name  : test.h
 * version    : 1.0
 * create date: 2016-10-18 16:55:12 +0800
 * creator    : tangyanmin@sensetime.com
 * description: TODO
 * modify log : {who} {when} {what}
 ***************************************************************************/

#ifndef SRC_TEST_H_
#define SRC_TEST_H_
#include <stdio.h>
#include <mongoc.h>
#include <mongoc-cursor.h>
#include <mongoc-collection.h>

static void mongo_iterator_collection_name( mongo *conn, const char *db)
{
         mongo_cursor cursor[1];
         char tmp[128];

         //要遍历数据库集合的命名空间为：数据库名.system.namespaces
        sprintf(tmp, "%s.system.namespaces", db);
        mongo_cursor_init( cursor, conn, tmp );
        while( mongo_cursor_next( cursor ) == MONGO_OK )
        {
               bson_iterator iterator[1];
               if ( bson_find( iterator, mongo_cursor_bson( cursor ), "name" ))
               {
                       const char *name = bson_iterator_string( iterator );
                       //过滤其他集合名
                       if(strstr(name, "system") != NULL || strstr(name, "$") != NULL)
                       {
                              continue;
                       }

                       //输出需要的集合名，结果以："数据库名.集合名"的形式输出
                       printf( "%s\n",  name);
                }
        }

        mongo_cursor_destroy( cursor );
}



//main 函数

int main()
{
       mongo conn;

       int status = mongo_client(&conn, "127.0.0.1", 27017 );
      if( status != MONGO_OK )
      {
            switch ( conn.err )
            {
                 case MONGO_CONN_NO_SOCKET:
                 {
                        printf( "no socket\n" );
                        return 1;
                  }
                 case MONGO_CONN_FAIL:
                 {
                       printf( "connection failed\n" );
                       return 1;
                 }
                 case MONGO_CONN_NOT_MASTER:
                 {
                       printf( "not master\n" );
                       return 1;
                  }
            }
       }

        //开始遍历
       mongo_iterator_collection_name(&conn, "test");

       mongo_destroy(&conn );

       return 0;
}


#endif /* SRC_TEST_H_ */
