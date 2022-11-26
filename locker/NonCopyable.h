/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-08 12:12:29
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-25 18:22:01
 * @FilePath: /lighthouse/WebServer/locker/NonCopyable.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H
class NonCopyable
{
public:
      NonCopyable() = default;
      ~NonCopyable() = default;

      NonCopyable(const NonCopyable &) = delete;
      NonCopyable(NonCopyable &&) = delete;

      NonCopyable &operator=(const NonCopyable &) = delete;
      NonCopyable &operator=(NonCopyable &&) = delete;
};
#endif