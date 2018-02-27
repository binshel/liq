/**
 * @file timer.h
 * @brief 提供定时器功能
 */

#include <queue>
#include <map>
#include <vector>
#include <functional>

namespace liq {

#define MINUTE_SECONDS      60
#define HOUR_SECONDS        (60 * MINUTE_SECONDS)
#define DAY_SECONDS         (24 * HOUR_SECONDS)
#define WEEK_SECONDS        (7 * DAY_SECONDS)

    typedef std::function<void(void)> fun_timer_cb;
    typedef int64_t timer_id_t;

    struct Timer;
    class TimerPCompare
    {
    public:
        bool operator ()(Timer* &lhs, Timer* &rhs) const;
    };

    class TimerManager 
    {
    public:
        TimerManager();
        int ontick();

        timer_id_t set_timer(int32_t seconds, const fun_timer_cb &cb);
        timer_id_t set_interval(int32_t seconds, const fun_timer_cb &cb);
        timer_id_t alarm_hour(int32_t offset, const fun_timer_cb &cb);
        timer_id_t alarm_day(int32_t offset, const fun_timer_cb &cb);
        timer_id_t alarm_week(int32_t offset, const fun_timer_cb &cb);
        timer_id_t alarm_month(int32_t offset, const fun_timer_cb &cb);
        void cancel_timer(timer_id_t id);

    protected:
        timer_id_t alarm_fix_timer(Timer *timer, int32_t interval, int32_t offset);
        int32_t month_days();
        int32_t month_days(int32_t year, int32_t month);
        bool try_next_timer(Timer *timer, int64_t now_sec);
        timer_id_t do_add_timer(Timer *timer);

    private:
        int64_t last_sec;
        std::map<int64_t, Timer*> timers;
        std::priority_queue<Timer*, std::vector<Timer*>, TimerPCompare> timer_queue;

    };
}
