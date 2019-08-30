#include "training/logged_stats.h"
using namespace marian;

struct CustStat : public LoggedStat {
    CustStat() : LoggedStat("C.:", "in.") {}
    float  r = 0.14;
    std::string format_value() const override { return std::to_string(r); }
    void reset() { r *= 2; }
};

void init_spd() {
  std::vector<spdlog::sink_ptr> sinks;
  auto stderr_sink = spdlog::sinks::stderr_sink_mt::instance();
    sinks.push_back(stderr_sink);
  auto logger = std::make_shared<spdlog::logger>("general", begin(sinks), end(sinks));
  spdlog::register_logger(logger);
}

int main() {

    init_spd();

    float v = 1;

    LoggingContainer lc({
        New<CustStat>(), 
       // makeLambdaStat("V=", [&v](){ return std::to_string(v); }, "u",[&v](){v=0;})
    });
    lc.add_lambda("V=", "{:.2f}", [&v](){return v;}, "u");
    lc.add_variable("Y=", "{:.1e}", v, " z", -1.0f);

    lc.log_info(); v++;
    lc.log_info(); v++;
    lc.reset_all();
    lc.log_info(); v++;
    lc.log_info(); v++;

    auto c = lc.to_map();
    for (auto &e : c) std::cerr << e.first << " " << e.second << "\n";

}
