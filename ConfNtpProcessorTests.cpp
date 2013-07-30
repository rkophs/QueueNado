
#include "ConfProcessorTests.h"
#include "ConfNtp.h"
#include "NtpMsg.pb.h"
#include <g2log.hpp>

TEST_F(ConfProcessorTests, ConfNtp_Initialize) {
  ConfNtp conf;
  ASSERT_EQ("conf/nm.yaml.Interface", conf.GetPath());
  ASSERT_EQ("/etc/ntp.conf", conf.GetNtpPath());
}

TEST_F(ConfProcessorTests, ConfNtp) {
  ConfNtp conf;
  ASSERT_EQ("conf/nm.yaml.Interface", conf.GetPath());
  ASSERT_EQ("/etc/ntp.conf", conf.GetNtpPath());
}


namespace {

  struct MockConfNtpToReadFile : public ConfNtp {

    MockConfNtpToReadFile() : ConfNtp() {
    }

    std::string GetFileContent() override {
      return ConfNtp::GetFileContent();
    }
  };
}

TEST_F(ConfProcessorTests, ConfNtp_ReadFile) {
  MockConfNtpToReadFile conf;
  auto content = conf.GetFileContent();
  ASSERT_FALSE(content.empty());
}

namespace {

  struct MockConfNtp : public ConfNtp {
    const std::string mContent;

    explicit MockConfNtp(const std::string content) : ConfNtp(), mContent(content) {
      ReadNtpConfFromFile();
    }
  protected:
    std::string GetFileContent() override {
      LOG(INFO) << "content is: " << mContent;
      return mContent;
    }
  };

  const std::string contentOK = {"driftfile /var/lib/ntp/drift \n" \
        "restrict 127.0.0.1\n" \
        "restrict -6 ::1\n" \
        "server 10.128.64.251\n" \
        "server 10.128.64.252\n" \
        "includefile /etc/ntp/crypto/pw\n" \
        "keys /etc/ntp/keys\n"};


  const std::string contentWithNoBackup = {"driftfile /var/lib/ntp/drift \n" \
        "restrict 127.0.0.1\n" \
        "restrict -6 ::1\n" \
        "server 10.128.64.251\n" \
        "# server 10.128.64.252\n" \
        "includefile /etc/ntp/crypto/pw\n" \
        "keys /etc/ntp/keys\n"};
  
    const std::string contentWithNoServers = {"driftfile /var/lib/ntp/drift \n" \
        "restrict 127.0.0.1\n" \
        "restrict -6 ::1\n" \
        "#server 10.128.64.251\n" \
        "# server 10.128.64.252\n" \
        "server\n" \
        "server \n" \
        "server # \n" \
        "includefile /etc/ntp/crypto/pw\n" \
        "keys /etc/ntp/keys\n"};
}//anonymous

TEST_F(ConfProcessorTests, ConfNtp_ReadContentsFromFile) {
  MockConfNtp conf(contentOK);
  ASSERT_EQ("10.128.64.251", conf.GetMasterServer());
  ASSERT_EQ("10.128.64.252", conf.GetBackupServer());
}

TEST_F(ConfProcessorTests, ConfNtp_ReadContentsFromFileWithCommentedOutBackup) {
  MockConfNtp conf(contentWithNoBackup);
  ASSERT_EQ("10.128.64.251", conf.GetMasterServer());
  ASSERT_EQ("", conf.GetBackupServer()); 
}

TEST_F(ConfProcessorTests, ConfNtp_ReadContentsFromFileWithNoServers) {
  MockConfNtp conf(contentWithNoServers);
  std::string empty;
  ASSERT_EQ(empty, conf.GetMasterServer());
  ASSERT_EQ(empty, conf.GetBackupServer()); 
}