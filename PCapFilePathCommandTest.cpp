#include "PCapFilePathCommand.h"
#include "PCapFilePathCommandTest.h"
#include "MockPCapFilePathCommand.h"
#include "DriveInfo.h"
#include "Crowbar.h"
#include "MockConf.h"
#include "FileIO.h"
#include "Conversion.h"
#include "StringFix.h"
#include "MockDriveInfo.h"

#ifdef LR_DEBUG
TEST_F(PCapFilePathCommandTest, NonPartitionedFilePath) {
   cmd.set_stringargone("123456789012345678901234567890123456");
   MockPCapFilePathCommand testCommand(cmd, autoManagedManager);
   autoManagedManager.SetSuccess(true);
   autoManagedManager.SetResult("test result");

   MockConf conf;
   conf.mOverrideGetFirstCaptureLocation = true;
   conf.mPCapCaptureLocations.push_back("/usr/local/probe/pcap");
   protoMsg::CommandRequest request;
   request.set_type(::protoMsg::CommandRequest_CommandType_PCAP_FILE_PATH);
   protoMsg::CommandReply reply = testCommand.Execute(conf);

   ASSERT_TRUE(reply.success());
   EXPECT_TRUE(reply.result().find("/usr/local/probe/pcap")!=std::string::npos) << "Cannot find pcap dir " << reply.result();
   EXPECT_TRUE(reply.result().find(cmd.stringargone())!=std::string::npos) << "Cannot find uuid filename " << reply.result();

}
TEST_F(PCapFilePathCommandTest, PartitionedFilePath) {
   cmd.set_stringargone("123456789012345678901234567890123400");
   MockPCapFilePathCommand testCommand(cmd, autoManagedManager);
   autoManagedManager.SetSuccess(true);
   autoManagedManager.SetResult("test result");

   MockConf conf;
   conf.mOverrideGetFirstCaptureLocation = true;
   conf.mPCapCaptureLocations.push_back("/usr/local/probe/pcap0");
   conf.mPCapCaptureLocations.push_back("/usr/local/probe/pcap1");
   protoMsg::CommandRequest request;
   request.set_type(::protoMsg::CommandRequest_CommandType_PCAP_FILE_PATH);
   protoMsg::CommandReply reply = testCommand.Execute(conf);

   ASSERT_TRUE(reply.success());
   EXPECT_TRUE(reply.result().find("/usr/local/probe/pcap0")!=std::string::npos) << "Cannot find pcap dir " << reply.result();
   EXPECT_TRUE(reply.result().find(cmd.stringargone())!=std::string::npos) << "Cannot find uuid filename " << reply.result();
   cmd.set_stringargone("123456789012345678901234567890123401");
   MockPCapFilePathCommand testCommand2(cmd, autoManagedManager);
   reply = testCommand2.Execute(conf);
   ASSERT_TRUE(reply.success());
   EXPECT_TRUE(reply.result().find("/usr/local/probe/pcap1")!=std::string::npos) << "Cannot find pcap dir " << reply.result();
   EXPECT_TRUE(reply.result().find(cmd.stringargone())!=std::string::npos) << "Cannot find uuid filename " << reply.result();

}
TEST_F(PCapFilePathCommandTest, EmptyPartitionedFilePath) {
   cmd.set_stringargone("123456789012345678901234567890123456");
   MockPCapFilePathCommand testCommand(cmd, autoManagedManager);
   autoManagedManager.SetSuccess(true);
   autoManagedManager.SetResult("test result");

   MockConf conf;
   conf.mOverrideGetFirstCaptureLocation = true;
   conf.mPCapCaptureLocations.push_back("");
   protoMsg::CommandRequest request;
   request.set_type(::protoMsg::CommandRequest_CommandType_PCAP_FILE_PATH);
   protoMsg::CommandReply reply = testCommand.Execute(conf);

   ASSERT_FALSE(reply.success());
   EXPECT_TRUE(reply.result().empty());

}
TEST_F(PCapFilePathCommandTest, InvalidUUID) {
   cmd.set_stringargone("");
   MockPCapFilePathCommand testCommand(cmd, autoManagedManager);
   autoManagedManager.SetSuccess(true);
   autoManagedManager.SetResult("test result");

   MockConf conf;
   conf.mOverrideGetFirstCaptureLocation = true;
   conf.mPCapCaptureLocations.push_back("/usr/local/probe/pcap");
   protoMsg::CommandRequest request;
   request.set_type(::protoMsg::CommandRequest_CommandType_PCAP_FILE_PATH);
   protoMsg::CommandReply reply = testCommand.Execute(conf);

   ASSERT_FALSE(reply.success());
   EXPECT_TRUE(reply.result().empty());

}
#else 
TEST_F(PCapFilePathCommandTest, NullTest) {
   EXPECT_TRUE(true);
}

#endif
