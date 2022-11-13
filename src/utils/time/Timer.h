namespace qs::utils
{

class Timer
{
public:
    Timer() = default;
    Timer(const Timer&) = delete;
    Timer(Timer&&) noexcept = default;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) noexcept = default;
    virtual ~Timer() = default;

    virtual auto start() -> void = 0;
    virtual auto stop() -> void = 0;

    [[nodiscard]]
    virtual auto hasTimeoutOccurred() const -> bool = 0;
    [[nodiscard]]
    virtual auto isRunning() const -> bool = 0;
};

}
