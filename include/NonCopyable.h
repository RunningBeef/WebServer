#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H
class NonCopyable{
public:
      NonCopyable() = default;
      ~NonCopyable() = default;

      NonCopyable(const NonCopyable & ) = delete;
      NonCopyable(const NonCopyable & ) = delete;
      NonCopyable(NonCopyable &&) = delete;

      NonCopyable & operator = (const NonCopyable &) = delete;
      NonCopyable & operator = (NonCopyable &&) = delete;
};
#endif