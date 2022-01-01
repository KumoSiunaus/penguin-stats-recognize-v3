#ifdef PENGUIN_DLL_EXPORTS
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

void _load_templ(const char* itemId, const cv::Mat& templimg)
{
    auto& resource = penguin::resource;
    if (!resource.contains<std::map<std::string, cv::Mat>>("item_templs")) {
        resource.add("item_templs", std::map<std::string, cv::Mat>());
    }
    auto& item_templs = resource.get<std::map<std::string, cv::Mat>>("item_templs");
    item_templs[itemId] = templimg;
}

extern "C" {
    void load_templ(const char* itemId, const uint8_t* buffer, size_t size)
{
    cv::Mat templimg = decode(buffer, size);
    _load_templ(itemId, templimg);
}
}

extern "C" {
    void load_templ_with_data(const char* itemId, int rows, int cols, int type, void* data)
{
    cv::Mat templimg(rows, cols, type, data);
    _load_templ(itemId, templimg);
}
}

const char* _recognize(const cv::Mat& img)
{
    int64 start, end;
    static std::string res;
    if (!penguin::env_check()) {
        res = "env check fail";
        return res.data();
    }

    start = cv::getTickCount();

    penguin::Result result { img };
    result.analyze();

    end = cv::getTickCount();

    dict report = result.report();
    report["md5"] = result.get_md5();
    report["fingerprint"] = result.get_fingerprint();
    report["cost"] = (end - start) / cv::getTickFrequency() * 1000;

    res = report.dump();

    return res.data();
}

extern "C" {
    const char* recognize(const uint8_t* buffer, size_t size)
{
    cv::Mat img = decode(buffer, size);
    return _recognize(img);
}
}

extern "C" {
    const char* recognize_with_data(int rows, int cols, int type, void* data)
{
    cv::Mat img(rows, cols, type, data);
    return _recognize(img);
}
}