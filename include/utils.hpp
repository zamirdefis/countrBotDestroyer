#pragma once

static const std::vector<char> nums = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

bool isNum(const std::string& s);

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp);