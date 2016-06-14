#ifndef __USER_H__
#define __USER_H__
#include "DLLDefines.h"
#include "ISerializable.h"
#include <string>

class EXPORT User : public ISerializable
{
private:

	char * _message;
	char * _username;
	char * _password;

public:
	//For position we use floats since are the smallest floating point type (in all platforms)
	//double could be as big as float or bigger we do not know so just use float
	float X, Y, Z;
	int64_t Id;

	/*get the size of the object when serialized*/
	virtual int32_t serializeSize() const override;

	//get the data char* of the current object
	virtual char*  serialize()  override;

	//get the data char* and fill the current object with its data
	virtual void deserialize(const char* dataToDeserialize) override;

	void setMessage(char * newMessage);
	void setPassword(char * newPassword);
	void setUsername(char * newPassword);

	const std::string getUserName() const { return std::string(_username); };
	const std::string getPass() const { return std::string(_password); };

	//not allow to modify the returned message, if wanna modify, use setMessage
	const char * getMessage() const { return _message; };

	User();
	~User();
};

#endif // !__USER_H__