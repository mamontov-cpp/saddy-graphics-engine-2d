#include "packer.h"

Packer::Packer()
{
    
}

void Packer::setOptions(QHash<QString, QVariant>* options)
{
    this->m_options = options;
}

Packer::~Packer()
{
    
}
