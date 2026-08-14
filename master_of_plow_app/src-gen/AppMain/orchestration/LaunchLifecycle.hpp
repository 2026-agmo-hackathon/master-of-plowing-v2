#ifndef APPMAIN_ORCHESTRATION_LAUNCHLIFECYCLE_HPP
#define APPMAIN_ORCHESTRATION_LAUNCHLIFECYCLE_HPP

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <utility>

namespace AppMain { namespace orchestration {

class LaunchLifecycle {
public:
    class Token {
    public:
        Token()=default;
        Token(const Token&)=delete;
        Token& operator=(const Token&)=delete;
        Token(Token&& other) noexcept : owner_(other.owner_) { other.owner_=nullptr; }
        Token& operator=(Token&& other) noexcept {
            if(this!=&other){release();owner_=other.owner_;other.owner_=nullptr;}
            return *this;
        }
        ~Token(){release();}
        explicit operator bool() const{return owner_!=nullptr;}
    private:
        friend class LaunchLifecycle;
        explicit Token(LaunchLifecycle* owner):owner_(owner){}
        void release(){if(owner_){owner_->leave();owner_=nullptr;}}
        LaunchLifecycle* owner_=nullptr;
    };

    Token tryEnter()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(closed_)return {};
        ++active_;
        return Token(this);
    }

    void closeAndWait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        closed_=true;
        cv_.wait(lock,[this]{return active_==0;});
    }

    bool closed() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return closed_;
    }

private:
    void leave()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(--active_==0)cv_.notify_all();
    }
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    bool closed_=false;
    std::size_t active_=0;
};

}} // namespace AppMain::orchestration
#endif
