/*
 * Copyright 2021 Ingemar Hedvall
 * SPDX-License-Identifier: MIT
 */
#include <gtest/gtest.h>

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem> 
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

#include "mdf/cryptoutil.h"
namespace {
constexpr std::string_view kTestFile = "k:/test/mdf/mdf4_1/ct/cyclelow.mf4";
}

namespace mdf::test {

TEST(CryptoUtil, CreateMd5FileChecksum)  // NOLINT
{
  bool skip_test;
  try {
    skip_test = !fs::exists(kTestFile);
  } catch (const std::exception&) {
    skip_test = true;
  }
  if (skip_test) {
    GTEST_SKIP();
  }

  // Test that it runs normally
  const auto md5_normal = CreateMd5FileString(kTestFile.data());
  EXPECT_TRUE(!md5_normal.empty());
  EXPECT_EQ(md5_normal.size(), 32U) << md5_normal;
  std::cout << "MD5: " << md5_normal << std::endl;

  // Check that it handles files missing
  const auto md5_abnormal = CreateMd5FileString("testXXX.exe");
  EXPECT_TRUE(md5_abnormal.empty());
}

}  // namespace mdf::test
