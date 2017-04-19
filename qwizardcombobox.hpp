#ifndef QWIZARDCOMBOBOX_HPP
#define QWIZARDCOMBOBOX_HPP

#include <QComboBox>

class QWizardCombobox : public QComboBox
{
        Q_OBJECT
        QVariant currentItemData;

    public:
        QWizardCombobox(QWidget* parent=0) : QComboBox(parent)
        {
            currentItemData=QVariant();
            connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIdxChanged(int)));
        }

        Q_PROPERTY(QVariant currentItemData READ getCurrentItemData WRITE setCurrentItemData)
        QVariant getCurrentItemData() const {return currentItemData;}

    public Q_SLOTS:
        void onCurrentIdxChanged(int idx)
        {
            setCurrentItemData(itemData(idx));
        }

        void setCurrentItemData(QVariant data)
        {
            if (data!=currentItemData)
            {
                currentItemData=data;
                currentItemDataChanged(currentItemData);
            }
        }

    Q_SIGNALS:
        void currentItemDataChanged(QVariant);
};

#endif // QWIZARDCOMBOBOX_HPP
