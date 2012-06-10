

#define _s( _log_ )															\
	assert(Renderer::m_log.get().is_open());						\
	Renderer::m_log.get()<< _log_ <<std::endl;

#define _d( _log_or_call_ )													\
	assert(Renderer::m_log.get().is_open());						\
	Renderer::m_log.get()<< #_log_or_call_ <<";"<<std::endl;		\
	_log_or_call_ ;

#define _t( _log_or_call_ )													\
	if(0) {																	\
		assert(Renderer::m_log.get().is_open());					\
		Renderer::m_log.get()<< _log_or_call_ <<std::endl;		\
	} else  {																\
		assert(Renderer::m_log.get().is_open());					\
		Renderer::m_log.get()<< #_log_or_call_ <<";"<<std::endl;	\
		_log_or_call_ ;														\
	}

// #define _S(_call_)  \
// 	Renderer::m_log.get()<<#_call_<<std::endl; my_##_call_ ; 
#define _S(_call_)  my_##_call_ ; 