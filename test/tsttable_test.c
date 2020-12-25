#include <time.h>
#include <string.h>

#include "cc_tsttable.h"
#include "CppUTest/TestHarness_c.h"

static CC_TSTTableConf conf;
static CC_TSTTable *tst;
static CC_TSTTableIter iter;
static int stat;

TEST_GROUP_C_SETUP(CC_TSTTableTestsConf)
{
    cc_tsttable_conf_init(&conf);
    stat = cc_tsttable_new_conf(&conf, &tst);
};

TEST_GROUP_C_TEARDOWN(CC_TSTTableTestsConf)
{
    cc_tsttable_destroy(tst);
};

TEST_C(CC_TSTTableTestsConf, CC_TSTTableNew)
{
    CHECK_EQUAL_C_INT(CC_OK, stat);
    CHECK_EQUAL_C_INT(0, cc_tsttable_size(tst));
};



TEST_GROUP_C_SETUP(CC_TSTTableTests)
{
    cc_tsttable_new(&tst);
};

TEST_GROUP_C_TEARDOWN(CC_TSTTableTests)
{
    cc_tsttable_destroy(tst);
};

TEST_C(CC_TSTTableTests, CC_TSTTableAdd)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    cc_tsttable_add(tst, "key", a);
    cc_tsttable_add(tst, "randomstring", b);
    cc_tsttable_add(tst, "5", c);

    CHECK_EQUAL_C_INT(3, cc_tsttable_size(tst));

    char *r;
    cc_tsttable_get(tst, "key", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, a);
};

TEST_C(CC_TSTTableTests, CC_TSTTableAddSubKeys)
{
    char pairs [16][2][20]  = {
        {"one", "111"},
        {"two", "222"},
        {"three", "333"},
        {"onee", "444"},
        {"onez", "555"},
        {"onec", "666"},
        {"oneea", "777"},
        {"oneec", "888"},
        {"oneez", "999"},
        {"onezz", "aaa"},
        {"oneza", "bbb"},
        {"onezk", "ccc"},
        {"onecc", "ddd"},
        {"oneca", "eee"},
        {"onecz", "fff"},
        {"oneck", "000"},
    };
    int n = 16;

    for(int i = 0; i < n; i++) {
        stat = cc_tsttable_add(tst, pairs[i][0], pairs[i][1]);
        CHECK_C(stat == CC_OK);
    }

    CHECK_EQUAL_C_INT(n, cc_tsttable_size(tst));

    for(int i = 0; i < n; i++) {
        char * value;
        stat = cc_tsttable_get(tst, pairs[i][0], (void**)&value);
        CHECK_C(stat == CC_OK);
        CHECK_EQUAL_C_STRING(value, pairs[i][1]);
    }
}

TEST_C(CC_TSTTableTests, CC_TSTTableReplacingAdd)
{
    char *a = "value";
    char *b = "cookies";
    char *c = "m31";
    char *d = "okay";

    cc_tsttable_add(tst, "key", a);
    cc_tsttable_add(tst, "randomstring", b);
    cc_tsttable_add(tst, "5", c);
    cc_tsttable_add(tst, "randomstring", d);

    CHECK_EQUAL_C_INT(3, cc_tsttable_size(tst));

    char *r;
    cc_tsttable_get(tst, "randomstring", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, d);
};

TEST_C(CC_TSTTableTests, CC_TSTTableRemove)
{
    cc_tsttable_add(tst, "one", "111");
    CHECK_EQUAL_C_INT(1, cc_tsttable_size(tst));
    cc_tsttable_remove(tst, "one", NULL);
    CHECK_EQUAL_C_INT(0, cc_tsttable_size(tst));

    cc_tsttable_add(tst, "five", "555");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");
    CHECK_EQUAL_C_INT(3, cc_tsttable_size(tst));
    cc_tsttable_remove(tst, "five", NULL);
    cc_tsttable_remove(tst, "three", NULL);
    cc_tsttable_remove(tst, "random", NULL);
    CHECK_EQUAL_C_INT(0, cc_tsttable_size(tst));


    char *a = "value";
    char *b = "cookies";
    char *c = "m31";

    cc_tsttable_add(tst, "key", a);
    cc_tsttable_add(tst, "randomstring", b);
    cc_tsttable_add(tst, "5", c);

    char *rm;
    cc_tsttable_remove(tst, "randomstring", (void*) &rm);
    CHECK_EQUAL_C_INT(2, cc_tsttable_size(tst));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, cc_tsttable_get(tst, "randomstring", (void*) &g));

    cc_tsttable_add(tst, "randomstring", b);

    char *r;
    cc_tsttable_get(tst, "randomstring", (void*) &r);
    CHECK_EQUAL_C_POINTER(r, b);
};

TEST_C(CC_TSTTableTests, CC_TSTTableRemoveAll)
{
    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "randomkey", "randomvalue");

    CHECK_EQUAL_C_INT(2, cc_tsttable_size(tst));

    cc_tsttable_remove_all(tst);
    CHECK_EQUAL_C_INT(0, cc_tsttable_size(tst));

    char *g;
    CHECK_EQUAL_C_INT(CC_ERR_KEY_NOT_FOUND, cc_tsttable_get(tst, "key", (void*) &g));
};

TEST_C(CC_TSTTableTests, CC_TSTTableGet)
{
    char *val = "567";

    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "aask", val);

    CHECK_EQUAL_C_INT(2, cc_tsttable_size(tst));

    void *ret;
    cc_tsttable_get(tst, "aask", &ret);
    CHECK_EQUAL_C_STRING(val, ret);

};

TEST_C(CC_TSTTableTests, CC_TSTTableSize)
{
    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "randomstring", "cookies");
    cc_tsttable_add(tst, "5", "asdf");

    CHECK_EQUAL_C_INT(3, cc_tsttable_size(tst));
};

TEST_C(CC_TSTTableTests, CC_TSTTableContainsKey)
{
    cc_tsttable_add(tst, "key", "value");
    cc_tsttable_add(tst, "randomstring", "cookies");
    cc_tsttable_add(tst, "5", "m31");

    CHECK_EQUAL_C_INT(1, cc_tsttable_contains_key(tst, "key"));

    cc_tsttable_remove(tst, "key", NULL);
    CHECK_EQUAL_C_INT(0, cc_tsttable_contains_key(tst, "key"));
};

TEST_C(CC_TSTTableTests, CC_TSTTableIterNextEmpty)
{
    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry * data;
    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
};

TEST_C(CC_TSTTableTests, CC_TSTTableIterNext)
{

    cc_tsttable_add(tst, "one", "111");

    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry * data;
    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "one");
    CHECK_EQUAL_C_STRING(data->value, "111");

    cc_tsttable_add(tst, "two", "222");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");

    cc_tsttable_iter_init(&iter, tst);
    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "one");
    CHECK_EQUAL_C_STRING(data->value, "111");

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "random");
    CHECK_EQUAL_C_STRING(data->value, "439834");

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "three");
    CHECK_EQUAL_C_STRING(data->value, "333");

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(data->key, "two");
    CHECK_EQUAL_C_STRING(data->value, "222");

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
};

TEST_C(CC_TSTTableTests, CC_TSTTableIterRemoveEmpty)
{
    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry * data;
    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
    stat = cc_tsttable_iter_remove(&iter, (void**)&data);
    CHECK_C(stat == CC_ERR_KEY_NOT_FOUND);
}


TEST_C(CC_TSTTableTests, CC_TSTTableIterRemove)
{
    cc_tsttable_add(tst, "five", "555");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");

    cc_tsttable_iter_init(&iter, tst);
    CC_TSTTableEntry * data;
    void * value;

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_INT(3, cc_tsttable_size(tst));
    stat = cc_tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(2, cc_tsttable_size(tst));
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(value, "555");

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_INT(2, cc_tsttable_size(tst));
    stat = cc_tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(1, cc_tsttable_size(tst));
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(value, "439834");

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_INT(1, cc_tsttable_size(tst));
    stat = cc_tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(0, cc_tsttable_size(tst));
    CHECK_C(stat == CC_OK);
    CHECK_EQUAL_C_STRING(value, "333");

    stat = cc_tsttable_iter_next(&iter, &data);
    CHECK_C(stat == CC_ITER_END);
    CHECK_EQUAL_C_INT(0, cc_tsttable_size(tst));
    stat = cc_tsttable_iter_remove(&iter, &value);
    CHECK_EQUAL_C_INT(0, cc_tsttable_size(tst));
    CHECK_C(stat == CC_ERR_KEY_NOT_FOUND);
}

TEST_C(CC_TSTTableTests, CC_TSTTableLoop)
{
    CC_TSTTableEntry * data;

    cc_tsttable_add(tst, "five", "555");
    cc_tsttable_add(tst, "three", "333");
    cc_tsttable_add(tst, "random", "439834");
    CHECK_EQUAL_C_INT(3, cc_tsttable_size(tst));


    char keys[3][20] = {"five", "random", "three"};
    char values[3][20] = {"555", "439834", "333"};
    int i;

    i = 0;
    cc_tsttable_iter_init(&iter, tst);
    while (cc_tsttable_iter_next(&iter, &data) != CC_ITER_END)
        CHECK_EQUAL_C_STRING(data->key, keys[i++]);
    CHECK_EQUAL_C_INT(i, 3);

    i = 0;
    cc_tsttable_iter_init(&iter, tst);
    while (cc_tsttable_iter_next(&iter, &data) != CC_ITER_END)
        CHECK_EQUAL_C_STRING(data->value, values[i++]);
    CHECK_EQUAL_C_INT(i, 3);

    i = 0;
    char * key, *value;
    CC_TSTTABLE_FOREACH(tst, key, value, {
            CHECK_EQUAL_C_STRING(key, keys[i]);
        CHECK_EQUAL_C_STRING(value, values[i]);
        i += 1;
    });
    CHECK_EQUAL_C_INT(i, 3);

}
