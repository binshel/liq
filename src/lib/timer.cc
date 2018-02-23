#include "liq/liq.h"
#include "liq/timer.h"
#include "liq/thread_pool.h"
#include "liq/logger.h"

namespace liq {

    enum TIMER_TYPE
    {
        TIMER_TIMER,
        TIMER_INTERVAL,
        ALARM_HOUR,
        ALARM_DAY,
        ALARM_WEEK,
        ALARM_MONTH,
        TIMER_Max
    };

    struct Timer 
    {
        enum TIMER_TYPE type;
        bool canceled;
        timer_id_t id;
        int64_t set;
        int64_t expires;
        fun_timer_cb cb;

        Timer(TIMER_TYPE type, int64_t set, const fun_timer_cb& cb)
            :type(type), canceled(false),
            set(set), cb(cb)
        {
            id = ++last_id;
        }

        static timer_id_t last_id;
    };
    timer_id_t Timer::last_id = 0;

    bool TimerPCompare::operator ()(Timer* &lhs, Timer* &rhs) const
    {
        return lhs->expires > rhs->expires;
    }
    TimerManager::TimerManager()
        :timers(), timer_queue(TimerPCompare())
    {
        last_sec = Liq::instance().now.tv_sec + Liq::instance().tm.tm_gmtoff;
    }

    int TimerManager::ontick()
    {
        int32_t count = 0;
        int64_t now_sec = Liq::instance().now.tv_sec + Liq::instance().tm.tm_gmtoff;
        int64_t tick = now_sec - last_sec;
        if (tick == 0) return 0;
        if (now_sec - last_sec != 1) {
            WARNF("timer tick shock %ld %ld %ld", now_sec, last_sec, tick);
        }
        last_sec = now_sec;
        do {
            if (this->timer_queue.size() < 1) break;
            Timer *timer = this->timer_queue.top();
            if (timer->expires > now_sec) break;
            this->timer_queue.pop();

            // maybe canceled
            if (timer->canceled) {
                delete timer;
                continue;
            }
            bool next = try_next_timer(timer, now_sec);
            if (!next) this->cancel_timer(timer->id);
            Liq::instance().thread_pool->spawn([timer, next]() {
                    CATCH(timer->cb());
                    if (!next) delete timer;
                });
            ++count;
        } while(true);
        return count;
    }

    timer_id_t TimerManager::set_timer(int32_t seconds, const fun_timer_cb &cb)
    {
        Timer *timer = new Timer(TIMER_TIMER, seconds, cb);
        timer->set = seconds;
        timer->expires = last_sec + seconds;
        return do_add_timer(timer);
    }
    timer_id_t TimerManager::set_interval(int32_t seconds, const fun_timer_cb &cb)
    {
        Timer *timer = new Timer(TIMER_INTERVAL, seconds, cb);
        timer->set = seconds;
        timer->expires = last_sec + seconds;
        return do_add_timer(timer);
    }
    timer_id_t TimerManager::alarm_hour(int32_t offset, const fun_timer_cb &cb)
    {
        Timer *timer = new Timer(ALARM_HOUR, offset, cb);
        return alarm_fix_timer(timer, HOUR_SECONDS, offset);
    }
    timer_id_t TimerManager::alarm_day(int32_t offset, const fun_timer_cb &cb)
    {
        Timer *timer = new Timer(ALARM_DAY, offset, cb);
        return alarm_fix_timer(timer, DAY_SECONDS, offset);
    }
    timer_id_t TimerManager::alarm_week(int32_t offset, const fun_timer_cb &cb)
    {
        Timer *timer = new Timer(ALARM_WEEK, offset, cb);
        return alarm_fix_timer(timer, WEEK_SECONDS, offset);
    }
    timer_id_t TimerManager::alarm_month(int32_t offset, const fun_timer_cb &cb)
    {
        Timer *timer = new Timer(ALARM_MONTH, offset, cb);
        int32_t now_offset = (Liq::instance().tm.tm_mday - 1) * DAY_SECONDS
                            + Liq::instance().tm.tm_hour * HOUR_SECONDS
                            + Liq::instance().tm.tm_min * MINUTE_SECONDS
                            + Liq::instance().tm.tm_sec;
        timer->expires = last_sec - now_offset + offset;
        if (timer->expires <= last_sec) {
            timer->expires += month_days() * DAY_SECONDS;
        }
        return timer->id;
    }

    timer_id_t TimerManager::alarm_fix_timer(Timer *timer, int32_t interval, int32_t offset)
    {
        timer->set = offset;
        timer->expires = (last_sec / interval) * interval + offset;
        if (timer->expires <= last_sec) {
            timer->expires += interval;
        }
        return do_add_timer(timer);
    }

    int32_t TimerManager::month_days()
    {
        int month = Liq::instance().tm.tm_mon;
        int year = Liq::instance().tm.tm_year + 1900;
        return month_days(year, month);
    }
    int32_t TimerManager::month_days(int32_t year, int32_t month)
    {
        static const int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int days = month_days[month];
        if (month == 1 && year % 4 == 0  && year % 400 != 0) ++days;
        return days;
    }
    bool TimerManager::try_next_timer(Timer *timer, int64_t now_sec)
    {
        switch (timer->type) {
        case TIMER_TIMER:
            return false;
        case TIMER_INTERVAL:
            timer->expires = now_sec + timer->set;
            do_add_timer(timer);
            return true;
        case ALARM_HOUR:
            timer->expires += HOUR_SECONDS;
            do_add_timer(timer);
            return true;
        case ALARM_DAY:
            timer->expires += DAY_SECONDS;
            do_add_timer(timer);
            return true;
        case ALARM_WEEK:
            timer->expires += WEEK_SECONDS;
            do_add_timer(timer);
            return true;
        case ALARM_MONTH:
            timer->expires += month_days() * DAY_SECONDS;
            do_add_timer(timer);
            return true;
        default:
            throw "timer type error";
            return false;
        }
    }

    timer_id_t TimerManager::do_add_timer(Timer *timer)
    {
        this->timers[timer->id] = timer;
        this->timer_queue.push(timer);
        return timer->id;
    }

    void TimerManager::cancel_timer(timer_id_t id)
    {
        auto it = this->timers.find(id);
        if (it == this->timers.end()) return ;
        it->second->canceled= true;
        this->timers.erase(it);
    }
}

