#include <vector>
#include <string>

class TUI {
    private:
        std::vector<std::string> m_select_options; // TODO: change this to options later
        bool m_is_running;

    public:
        TUI();
        void run();
        bool is_running() const;
};