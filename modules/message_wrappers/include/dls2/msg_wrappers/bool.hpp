
#ifndef BOOL_HPP
#define BOOL_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/bool.h"

namespace dls
{
    class Bool : public Wrapper<BoolMsg>
    {
    public:
        template <typename SignalType>
        friend class SignalWriter;
        friend class PluginBase;
        Bool();
        Bool(const Bool &Bool);
        virtual ~Bool();

        operator BoolMsg() const override;
        Bool &operator=(const BoolMsg &Bool_msg) override;
        Bool &operator=(const Bool &Bool);

        std::string frame_id_{};
        uint32_t sequence_id_{};
        double timestamp_{};

        bool data_;

    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
    };
} // namespace dls

#endif