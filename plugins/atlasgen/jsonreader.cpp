#include "jsonreader.h"

#include <QtCore/QTextStream>

JSONReader::JSONReader()
{

}

JSONReader::~JSONReader()
{

}

void JSONReader::read(const QString& name)
{
	this->Successfull = false;
	QFile file(name); 
	if (!file.open(QIODevice::ReadOnly)) {
		this->Errors << QString("Can\'t open file \"") + name + QString("\"");
		Result->Errors << this->Errors;
		return;
	}
	QTextStream in(&file);
	QString contents = in.readAll();
	file.close();

	picojson::value root = picojson::parse_string(contents.toStdString());
	if (picojson::get_last_error().size() == 0)
	{
		if (root.is<picojson::object>())
		{
			picojson::object ro = root.get<picojson::object>();

			// Parse texture
			if (ro.find("texure") != ro.end())
			{
				if (ro["texture"].is<std::string>())
				{
					this->OutputTexture = ro["texture"].get<std::string>().c_str();
				}
				else
				{
					this->Errors << "Output texture is not a string";
				}
			}
			else
			{
				this->Errors << "Output texture is not defined";
			}


			// Parse config
			if (ro.find("config") != ro.end())
			{
				if (ro["config"].is<std::string>())
				{
					this->OutputName = ro["config"].get<std::string>().c_str();
				}
				else
				{
					this->Errors << "Output atlas config filename is not a string";
				}
			}
			else
			{
				this->Errors << "Output atlas config filename is not defined";
			}

		}
		else
		{
			this->Errors << QString("Root element of file \"") + name + QString("\" is not an object");
		}
	}
	else
	{
		this->Errors << QString("Can\'t parse file \"") + name + QString("\"");
	}

	this->Successfull = this->Errors.size() == 0;
	Result->Errors << this->Errors;
}
