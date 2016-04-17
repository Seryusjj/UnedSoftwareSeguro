#ifndef __USER_H__
#define __USER_H__
#include "DLLDefines.h"
#include "ISerializable.h"

class EXPORT User : public ISerializable
{
private:

public:
	virtual size_t serializeSize() const override;

	//get an ISerializable* and return the char*
	virtual char*  serialize()  override;

	//get the data char* and return a ISerializable*
	virtual void deserialize(const char* dataToDeserialize) override;

	int posX;
	int posY;
	int posZ;
	char* message;

	User();
	~User();
};

#endif // !__USER_H__