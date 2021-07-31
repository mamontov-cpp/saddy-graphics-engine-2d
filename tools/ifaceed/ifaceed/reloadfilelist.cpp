#include <reloadfilelist.h>

#include <renderer.h>

#include <resource/tree.h>

Q_DECLARE_METATYPE(sad::resource::ResourceFile*); //-V566

ReloadFileList::ReloadFileList(QWidget* parent) : QDialog(parent)
{
    m_ui.setupUi(this);

    const sad::Vector<sad::resource::ResourceFile*> & files = sad::Renderer::ref()->tree("")->files();
    for(size_t i = 0; i < files.size(); i++)
    {
        m_ui.lstFiles->addItem(files[i]->name().c_str());
        QVariant v;
        v.setValue(files[i]);
        m_ui.lstFiles->item(m_ui.lstFiles->count() - 1)->setData(Qt::UserRole, v);
    }


    connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_ui.cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

sad::resource::ResourceFile* ReloadFileList::selectedFile()
{
    QListWidgetItem* item = m_ui.lstFiles->currentItem();
    sad::resource::ResourceFile* result = nullptr;
    if (item)
    {
        result =  item->data(Qt::UserRole).value<sad::resource::ResourceFile*>();
    }
    return result;
}

ReloadFileList::~ReloadFileList()
{
    
}
