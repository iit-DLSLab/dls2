/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include <streambuf>

namespace dls
{
namespace logging
{
class LogStreamBuffer : public std::streambuf
{
public:
	LogStreamBuffer(std::size_t buffer_size = 512);
	~LogStreamBuffer();

	LogStreamBuffer(const LogStreamBuffer&) = delete;
	LogStreamBuffer &operator=(const LogStreamBuffer&) = delete;

private:
	int_type overflow(int_type ch) override;
	int sync() override;
	bool flush_buffer();

	char *buf;
};

extern std::ostream cdbg;
extern std::ostream clog;
extern std::ostream cout;
extern std::ostream cerr;
extern std::ostream cfatal;

} // namespace logging
} // namespace dls

#endif /* end of include guard: LOG_HPP_AMCC9JXS */
