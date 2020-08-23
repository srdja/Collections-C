#include <time.h>
#include <string.h>

#include "tsttable.h"
#include "CppUTest/TestHarness_c.h"

static TSTTableConf conf;
static TSTTable *tst;
static TSTTableIter iter;
static int stat;

TEST_GROUP_C_SETUP(TSTTableTestsConf)
{
    tsttable_conf_init(&conf);
    stat = tsttable_new_conf(&conf, &tst);
};

TEST_GROUP_C_TEARDOWN(TSTTableTestsConf)
{
    tsttable_destroy(tst);
};

TEST_C(TSTTableTestsConf, TSTTableNew)
{
    CHECK_EQUAL_C_INT(CC_OK, stat);
    CHECK_EQUAL_C_INT(0, tsttable_size(tst));
};



TEST_GROUP_C_SETUP(TSTTableTests)
{
    tsttable_new(&tst);
};

TEST_GROUP_C_TEARDOWN(TSTTableTests)
{
    tsttable_destroy(tst);
};

TEST_C(TSTTableTests, TSTTableAdd)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    tsttable_add(tst, "key", a);
    tsttable_add(tst, "randomstring", b);
    tsttable_add(tst, "5", c);

    CHECK_EQUAL_C_INT(3, tsttable_size(tst));

    char *r;
    tsttable_get(tst, "key", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, a);
};

TEST_C(TSTTableTests, TSTTableReplacingAdd)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";
    char *d = "okay";

    tsttable_add(tst, "key", a);
    tsttable_add(tst, "randomstring", b);
    tsttable_add(tst, "5", c);
    tsttable_add(tst, "randomstring", d);

    CHECK_EQUAL_C_INT(3, tsttable_size(tst));

    char *r;
    tsttable_get(tst, "randomstring", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, d);
};

TEST_C(TSTTableTests, TSTTableRemove)
{
    tsttable_add(tst, "one", "111");
    CHECK_EQUAL_C_INT(1, tsttable_size(tst));
    tsttable_remove(tst, "one", NULL);
    CHECK_EQUAL_C_INT(0, tsttable_size(tst));

    tsttable_add(tst, "five", "555");
    tsttable_add(tst, "three", "333");
    tsttable_add(tst, "random", "439834");
    CHECK_EQUAL_C_INT(3, tsttable_size(tst));
    tsttable_remove(tst, "five", NULL);
    tsttable_remove(tst, "three", NULL);
    tsttable_remove(tst, "random", NULL);
    CHECK_EQUAL_C_INT(0, tsttable_size(tst));


    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    tsttable_add(tst, "key", a);
    tsttable_add(tst, "randomstring", b);
    tsttable_add(tst, "5", c);

    char *rm;
    tsttable_remove(tst, "randomstring", (void*) &rm);
    CHECK_EQUAL_C_INT(2, tsttable_size(tst));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, tsttable_get(tst, "randomstring", (void*) &g));

    tsttable_add(tst, "randomstring", b);

    char *r;
    tsttable_get(tst, "randomstring", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, b);
};

TEST_C(TSTTableTests, TSTTableRemoveAll)
{
    tsttable_add(tst, "key", "value");
    tsttable_add(tst, "randomkey", "randomvalue");

    CHECK_EQUAL_C_INT(2, tsttable_size(tst));

    tsttable_remove_all(tst);
    CHECK_EQUAL_C_INT(0, tsttable_size(tst));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, tsttable_get(tst, "key", (void*) &g));
};

TEST_C(TSTTableTests, TSTTableGet)
{
    char *val = "567";

    tsttable_add(tst, "key", "value");
    tsttable_add(tst, "aask", val);

    CHECK_EQUAL_C_INT(2, tsttable_size(tst));

    void *ret;
    tsttable_get(tst, "aask", &ret);
    CHECK_EQUAL_C_STRING(val, ret);

};

TEST_C(TSTTableTests, TSTTableSize)
{
    tsttable_add(tst, "key", "value");
    tsttable_add(tst, "randomstring", "cookies");
    tsttable_add(tst, "5", "asdf");

    CHECK_EQUAL_C_INT(3, tsttable_size(tst));
};
 
TEST_C(TSTTableTests, TSTTableContainsKey)
{
    tsttable_add(tst, "key", "value");
    tsttable_add(tst, "randomstring", "cookies");
    tsttable_add(tst, "5", "m31");

    CHECK_EQUAL_C_INT(1, tsttable_contains_key(tst, "key"));

    tsttable_remove(tst, "key", NULL);
    CHECK_EQUAL_C_INT(0, tsttable_contains_key(tst, "key"));
};

TEST_C(TSTTableTests, TSTTableIterNextEmpty)
{
    tsttable_iter_init(&iter, tst);
    TSTTableEntry * data;
    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
};

TEST_C(TSTTableTests, TSTTableIterNext)
{
    
    tsttable_add(tst, "one", "111");

    tsttable_iter_init(&iter, tst);
    TSTTableEntry * data;
    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "one");
    CHECK_EQUAL_C_STRING(data->value, "111");

    tsttable_add(tst, "two", "222");
    tsttable_add(tst, "three", "333");
    tsttable_add(tst, "random", "439834");

    tsttable_iter_init(&iter, tst);
    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "one");
    CHECK_EQUAL_C_STRING(data->value, "111");

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "random");
    CHECK_EQUAL_C_STRING(data->value, "439834");

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "three");
    CHECK_EQUAL_C_STRING(data->value, "333");

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "two");
    CHECK_EQUAL_C_STRING(data->value, "222");

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
};

TEST_C(TSTTableTests, TSTTableIterRemoveEmpty)
{
    tsttable_iter_init(&iter, tst);
    TSTTableEntry * data;
    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
    stat = tsttable_iter_remove(&iter, (void**)&data);
    CHECK_C(stat == CC_ERR_KEY_NOT_FOUND);
}


TEST_C(TSTTableTests, TSTTableIterRemove)
{
    tsttable_add(tst, "five", "555");
    tsttable_add(tst, "three", "333");
    tsttable_add(tst, "random", "439834");

    tsttable_iter_init(&iter, tst);
    TSTTableEntry * data;
    void * value;

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_INT(3, tsttable_size(tst));
    stat = tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(2, tsttable_size(tst));
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(value, "555");

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_INT(2, tsttable_size(tst));
    stat = tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(1, tsttable_size(tst));
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(value, "439834");

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_INT(1, tsttable_size(tst));
    stat = tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(0, tsttable_size(tst));
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(value, "333");

    stat = tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
    CHECK_EQUAL_C_INT(0, tsttable_size(tst));
    stat = tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(0, tsttable_size(tst));
    CHECK_C(stat == CC_ERR_KEY_NOT_FOUND);       
}

TEST_C(TSTTableTests, TSTTableLoop)
{
    TSTTableEntry * data;
    
    tsttable_add(tst, "five", "555");
    tsttable_add(tst, "three", "333");
    tsttable_add(tst, "random", "439834");
    CHECK_EQUAL_C_INT(3, tsttable_size(tst));
    

    char keys[3][20] = {"five", "random", "three"};
    char values[3][20] = {"555", "439834", "333"};
    int i;

    i = 0;
    tsttable_iter_init(&iter, tst);
    while (tsttable_iter_next(&iter, &data) != CC_ITER_END)
        CHECK_EQUAL_C_STRING(data->key, keys[i++]);
    CHECK_EQUAL_C_INT(i, 3);

    i = 0;
    tsttable_iter_init(&iter, tst);
    while (tsttable_iter_next(&iter, &data) != CC_ITER_END)
        CHECK_EQUAL_C_STRING(data->value, values[i++]);
    CHECK_EQUAL_C_INT(i, 3);

    i = 0;
    char * key, *value;
    TSTTABLE_FOREACH(tst, key, value, {
        CHECK_EQUAL_C_STRING(key, keys[i]);
        CHECK_EQUAL_C_STRING(value, values[i]);
        i += 1;
    });
    CHECK_EQUAL_C_INT(i, 3);

}