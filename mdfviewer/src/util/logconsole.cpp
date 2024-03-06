/*
 * Copyright 2021 Ingemar Hedvall
 * SPDX-License-Identifier: MIT
 */
#include "logconsole.h"

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem> 
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif
#include <iostream>

#include "timestamp.h"

namespace {

std::string GetStem(const std::string &file) {
  try {
   fs::path p(file);
    return p.stem().string();
  } catch (const std::exception &) {
  }
  return file;
}
}  // namespace

namespace util::log::detail {

void LogConsole::AddLogMessage(const LogMessage &message) {
  if (message.message.empty() || !IsSeverityLevelEnabled(message.severity)) {
    return;
  }

  const char last = message.message.back();
  const bool has_newline = last == '\n' || last == '\r';
  const std::string time = time::GetLocalTimestampWithMs(message.timestamp);
  const std::string severity = GetSeverityString(message.severity);

  std::lock_guard<std::mutex> guard(locker_);  // Fix multi-thread issue

  std::clog << "[" << time << "] " << severity << " " << message.message;
  if (ShowLocation()) {
    std::clog << " [" << GetStem(message.location.file_name()) << ":"
              << message.location.function_name() << ":"
              << message.location.line() << "]";
  }
  if (!has_newline) {
    std::clog << std::endl;
  }
  std::clog.flush();
}

}  // namespace util::log::detail