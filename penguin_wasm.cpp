﻿#ifdef PENGUIN_DLL_EXPORTS
#include "penguin.h"
#endif

#include "result.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat decode(const uint8_t* buffer, size_t size)
{
    std::vector buf(buffer, buffer + size);
    return cv::imdecode(buf, 1);
}

extern "C" {
const char* get_info()
{
    const static std::string version = "3.2.2";
    return version.c_str();
}
}

extern "C" {
    void load_server(const char* server)
{
    penguin::server = server;
}
}

extern "C" {
    void load_json(const char* stage_index, const char* hash_index)
{
    auto& resource = penguin::resource;
    resource.add("stage_index", dict::parse(stage_index));
    resource.add("hash_index", dict::parse(hash_index));
}
}

extern "C" {
    void load_templ(const char* itemId, const uint8_t* buffer, size_t size)
{
    cv::Mat templimg = decode(buffer, size);
    auto& resource = penguin::resource;
    if (!resource.contains<std::map<std::string, cv::Mat>>("item_templs")) {
        resource.add("item_templs", std::map<std::string, cv::Mat>());
    }
    auto& item_templs = resource.get<std::map<std::string, cv::Mat>>("item_templs");
    item_templs[itemId] = templimg;
}
}

extern "C" {
    const char* recognize(const uint8_t* buffer, size_t size)
{
    int64 start, end;
    static std::string res;
    if (!penguin::env_check()) {
        res = "env check fail";
        return res.data();
    }

    start = cv::getTickCount();

    cv::Mat img = decode(buffer, size);
    penguin::Result result { img };
    result.analyze();

    end = cv::getTickCount();

    dict report = result.report();
    report["md5"] = result.get_md5();
    report["fingerprint"] = result.get_fingerprint();
    report["cost"] = (end - start) / cv::getTickFrequency() * 1000;

    res = report.dump();

    // free(buffer);
    return res.data();
}
}
