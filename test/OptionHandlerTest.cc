#include "OptionHandlerImpl.h"
#include "prefs.h"
#include "Exception.h"
#include <cppunit/extensions/HelperMacros.h>

class OptionHandlerTest:public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(OptionHandlerTest);
  CPPUNIT_TEST(testNullOptionHandler);
  CPPUNIT_TEST(testBooleanOptionHandler);
  CPPUNIT_TEST(testNumberOptionHandler);
  CPPUNIT_TEST(testNumberOptionHandler_min);
  CPPUNIT_TEST(testNumberOptionHandler_max);
  CPPUNIT_TEST(testNumberOptionHandler_min_max);
  CPPUNIT_TEST(testUnitNumberOptionHandler);
  CPPUNIT_TEST(testParameterOptionHandler_1argInit);
  CPPUNIT_TEST(testParameterOptionHandler_2argsInit);
  CPPUNIT_TEST(testParameterOptionHandler_listInit);
  CPPUNIT_TEST(testDefaultOptionHandler);
  CPPUNIT_TEST(testFloatNumberOptionHandler);
  CPPUNIT_TEST(testFloatNumberOptionHandler_min);
  CPPUNIT_TEST(testFloatNumberOptionHandler_max);
  CPPUNIT_TEST(testFloatNumberOptionHandler_min_max);
  CPPUNIT_TEST(testLogOptionHandler);
  CPPUNIT_TEST(testHttpProxyOptionHandler);
  CPPUNIT_TEST_SUITE_END();
  
public:
  void testNullOptionHandler();
  void testBooleanOptionHandler();
  void testNumberOptionHandler();
  void testNumberOptionHandler_min();
  void testNumberOptionHandler_max();
  void testNumberOptionHandler_min_max();
  void testUnitNumberOptionHandler();
  void testParameterOptionHandler_1argInit();
  void testParameterOptionHandler_2argsInit();
  void testParameterOptionHandler_listInit();
  void testDefaultOptionHandler();
  void testFloatNumberOptionHandler();
  void testFloatNumberOptionHandler_min();
  void testFloatNumberOptionHandler_max();
  void testFloatNumberOptionHandler_min_max();
  void testLogOptionHandler();
  void testHttpProxyOptionHandler();
};


CPPUNIT_TEST_SUITE_REGISTRATION( OptionHandlerTest );

void OptionHandlerTest::testNullOptionHandler()
{
  NullOptionHandler handler;
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  handler.parseArg(0, "bar");
}

void OptionHandlerTest::testBooleanOptionHandler()
{
  BooleanOptionHandler handler("foo");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, V_TRUE);
  CPPUNIT_ASSERT_EQUAL(string(V_TRUE), option.get("foo"));
  handler.parseArg(&option, V_FALSE);
  CPPUNIT_ASSERT_EQUAL(string(V_FALSE), option.get("foo"));
  try {
    handler.parseArg(&option, "hello");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testNumberOptionHandler()
{
  NumberOptionHandler handler("foo");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "0");
  CPPUNIT_ASSERT_EQUAL(string("0"), option.get("foo"));
}

void OptionHandlerTest::testNumberOptionHandler_min()
{
  NumberOptionHandler handler("foo", 1);
  Option option;
  handler.parseArg(&option, "1");
  CPPUNIT_ASSERT_EQUAL(string("1"), option.get("foo"));
  try {
    handler.parseArg(&option, "0");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }

}

void OptionHandlerTest::testNumberOptionHandler_max()
{
  NumberOptionHandler handler("foo", -1, 100);
  Option option;
  handler.parseArg(&option, "100");
  CPPUNIT_ASSERT_EQUAL(string("100"), option.get("foo"));
  try {
    handler.parseArg(&option, "101");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testNumberOptionHandler_min_max()
{
  NumberOptionHandler handler("foo", 1, 100);
  Option option;
  handler.parseArg(&option, "1");
  CPPUNIT_ASSERT_EQUAL(string("1"), option.get("foo"));
  handler.parseArg(&option, "100");
  CPPUNIT_ASSERT_EQUAL(string("100"), option.get("foo"));
  try {
    handler.parseArg(&option, "0");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
  try {
    handler.parseArg(&option, "101");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testUnitNumberOptionHandler()
{
  UnitNumberOptionHandler handler("foo");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "4294967296");
  CPPUNIT_ASSERT_EQUAL(string("4294967296"), option.get("foo"));
  handler.parseArg(&option, "4096M");
  CPPUNIT_ASSERT_EQUAL(string("4294967296"), option.get("foo"));
  handler.parseArg(&option, "4096K");
  CPPUNIT_ASSERT_EQUAL(string("4194304"), option.get("foo"));
  handler.parseArg(&option, "K");
  CPPUNIT_ASSERT_EQUAL(string("0"), option.get("foo"));
  handler.parseArg(&option, "M");
  CPPUNIT_ASSERT_EQUAL(string("0"), option.get("foo"));
  handler.parseArg(&option, "");
  CPPUNIT_ASSERT_EQUAL(string("0"), option.get("foo"));
}

void OptionHandlerTest::testParameterOptionHandler_1argInit()
{
  ParameterOptionHandler handler("foo", "value1");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "value1");
  CPPUNIT_ASSERT_EQUAL(string("value1"), option.get("foo"));
  try {
    handler.parseArg(&option, "value3");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testParameterOptionHandler_2argsInit()
{
  ParameterOptionHandler handler("foo", "value1", "value2");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "value1");
  CPPUNIT_ASSERT_EQUAL(string("value1"), option.get("foo"));
  handler.parseArg(&option, "value2");
  CPPUNIT_ASSERT_EQUAL(string("value2"), option.get("foo"));
  try {
    handler.parseArg(&option, "value3");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testParameterOptionHandler_listInit()
{
  Strings validValues;
  validValues.push_back("value1");
  validValues.push_back("value2");

  ParameterOptionHandler handler("foo", validValues);
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "value1");
  CPPUNIT_ASSERT_EQUAL(string("value1"), option.get("foo"));
  handler.parseArg(&option, "value2");
  CPPUNIT_ASSERT_EQUAL(string("value2"), option.get("foo"));
  try {
    handler.parseArg(&option, "value3");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testDefaultOptionHandler()
{
  DefaultOptionHandler handler("foo");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "bar");
  CPPUNIT_ASSERT_EQUAL(string("bar"), option.get("foo"));
  handler.parseArg(&option, "");
  CPPUNIT_ASSERT_EQUAL(string(""), option.get("foo"));
}

void OptionHandlerTest::testFloatNumberOptionHandler()
{
  FloatNumberOptionHandler handler("foo");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "1.0");
  CPPUNIT_ASSERT_EQUAL(string("1.0"), option.get("foo"));
}

void OptionHandlerTest::testFloatNumberOptionHandler_min()
{
  FloatNumberOptionHandler handler("foo", 0.0);
  Option option;
  handler.parseArg(&option, "0.0");
  CPPUNIT_ASSERT_EQUAL(string("0.0"), option.get("foo"));
  try {
    handler.parseArg(&option, "-0.1");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testFloatNumberOptionHandler_max()
{
  FloatNumberOptionHandler handler("foo", -1, 10.0);
  Option option;
  handler.parseArg(&option, "10.0");
  CPPUNIT_ASSERT_EQUAL(string("10.0"), option.get("foo"));
  try {
    handler.parseArg(&option, "10.1");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testFloatNumberOptionHandler_min_max()
{
  FloatNumberOptionHandler handler("foo", 0.0, 10.0);
  Option option;
  handler.parseArg(&option, "0.0");
  CPPUNIT_ASSERT_EQUAL(string("0.0"), option.get("foo"));
  handler.parseArg(&option, "10.0");
  CPPUNIT_ASSERT_EQUAL(string("10.0"), option.get("foo"));
  try {
    handler.parseArg(&option, "-0.1");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
  try {
    handler.parseArg(&option, "10.1");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}

void OptionHandlerTest::testLogOptionHandler()
{
  LogOptionHandler handler("foo");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "/tmp/log.txt");
  CPPUNIT_ASSERT_EQUAL(string("/tmp/log.txt"), option.get(PREF_LOG));
  CPPUNIT_ASSERT_EQUAL(string(""), option.get(PREF_STDOUT_LOG));

  option.clear();
  handler.parseArg(&option, "-");
  CPPUNIT_ASSERT_EQUAL(string(""), option.get(PREF_LOG));
  CPPUNIT_ASSERT_EQUAL(string(V_TRUE), option.get(PREF_STDOUT_LOG));
}

void OptionHandlerTest::testHttpProxyOptionHandler()
{
  HttpProxyOptionHandler handler("foo");
  CPPUNIT_ASSERT(handler.canHandle("foo"));
  CPPUNIT_ASSERT(!handler.canHandle("foobar"));
  Option option;
  handler.parseArg(&option, "bar:80");
  CPPUNIT_ASSERT_EQUAL(string("bar:80"), option.get(PREF_HTTP_PROXY));
  CPPUNIT_ASSERT_EQUAL(string("bar"), option.get(PREF_HTTP_PROXY_HOST));
  CPPUNIT_ASSERT_EQUAL(string("80"), option.get(PREF_HTTP_PROXY_PORT));
  CPPUNIT_ASSERT_EQUAL(string(V_TRUE), option.get(PREF_HTTP_PROXY_ENABLED));

  try {
    handler.parseArg(&option, "bar");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
  try {
    handler.parseArg(&option, "bar:");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
  try {
    handler.parseArg(&option, ":");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
  try {
    handler.parseArg(&option, ":80");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
  try {
    handler.parseArg(&option, "foo:bar");
    CPPUNIT_FAIL("exception must be threw.");
  } catch(Exception* e) {
    cerr << e->getMsg() << endl;
    delete e;
  }
}
