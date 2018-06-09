#pragma once
namespace Arko
{
  class Exception
  {
  public:
  	explicit Exception(const char*message):
  		msg(message)
  	{}
  	explicit Exception(const std::string&message):
  		msg(message)
  	{}
  	virtual ~Exception(){}
  	virtual const char*what()const
  	{
  		return msg.c_str();
  	}
  protected:
  	std::string msg;
  };
}
