#include "gtest/gtest.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
using org::esb::av::Decoder;
using org::esb::av::Encoder;
class DecoderTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  DecoderTest() {
    // You can do set-up work for each test here.
  }

  virtual ~DecoderTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    dec=new Decoder(CODEC_ID_MPEG4);
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    delete dec;
  }
  Decoder *dec;
  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(DecoderTest, DoesOpen) {
  //dec->open();
}
