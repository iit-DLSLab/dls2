#ifndef _LOGGER_H_
#define _LOGGER_H_

class Logger
{
public:
    /**
      *@brief Logger constructor
      */
    Logger() {}

    /**
      *@brief
      */
    ~Logger() {}

    /**
     * @brief writeOutDebugVars
     * @param debug_vars
     */
     inline void writeOutDebugVars(std::vector<std::pair<std::string, double>>& debug_vars)
     {
            if(!debug.empty())
                    debug_vars = debug;
     }

    /**
     * @brief debugPush
     * @param var_name
     * @param var_value
     */
    inline void debugPush(std::string var_name, double var_value)
    {
        if (debug_iter >= debug.size())
            debug.push_back(std::make_pair(var_name, var_value));
        else
            debug[debug_iter++] = std::make_pair(var_name, var_value);
    }

    /**
     * @brief debugClear
     */
    inline void debugClear() { debug_iter = 0; }

 private:
    /** @brief debug vector of variables (for logging and plotting) */
    std::vector<std::pair<std::string,double>> debug;

    /** @brief debug vector iterator */
    unsigned int debug_iter = 0;
};

#endif
