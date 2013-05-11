#include "abstractblock.hpp"

CSVSettings::AbstractBlock::AbstractBlock(QWidget* parent)
    : QObject (parent), mBox ( new GroupBox (parent) ), mWidgetParent (parent)
{}

CSVSettings::AbstractBlock::AbstractBlock(bool isVisible, QWidget* parent)
    : QObject (parent), mBox ( new GroupBox (isVisible, parent)), mWidgetParent (parent)
{}

QLayout *CSVSettings::AbstractBlock::createLayout (OcsWidgetOrientation direction,
                                                   bool isZeroMargin, QWidget* parent)
{
    QLayout *layout = 0;

    if (direction == OCS_VERTICAL)
        layout = new QVBoxLayout (parent);
    else
        layout = new QHBoxLayout (parent);

    if (isZeroMargin)
        layout->setContentsMargins(0, 0, 0, 0);

    return layout;
}

QGroupBox *CSVSettings::AbstractBlock::getGroupBox()
{
    return mBox;
}

CSVSettings::AbstractWidget *CSVSettings::AbstractBlock::buildWidget (const QString& widgetName, WidgetDef &def,
                                                                        QLayout *layout, bool isConnected) const
{
    AbstractWidget *widg = 0;

    switch (def.type)
    {

    case OCS_RADIO_WIDGET:
        widg = createSettingWidget<QRadioButton> (def, layout);
        break;

    case OCS_SPIN_WIDGET:
        widg = createSettingWidget<QSpinBox> (def, layout);
        break;

    case OCS_CHECK_WIDGET:
        widg = createSettingWidget<QCheckBox> (def, layout);
        break;

    case OCS_TEXT_WIDGET:
        widg = createSettingWidget<QLineEdit> (def, layout);
        break;

    case OCS_LIST_WIDGET:
        widg = createSettingWidget<QListWidget> (def, layout);
        break;

    case OCS_COMBO_WIDGET:
        widg = createSettingWidget<QComboBox> (def, layout);
        break;

    default:
        break;
    };

    if (!mBox->layout())
        mBox->setLayout(widg->getLayout());

    widg->widget()->setObjectName(widgetName);

    if (isConnected)
        connect (widg, SIGNAL (signalUpdateItem (const QString &)), this, SLOT (slotUpdate (const QString &)));
        connect (this, SIGNAL (signalUpdateWidget (const QString &)), widg, SLOT (slotUpdateWidget (const QString &) ));

    return widg;
}

void CSVSettings::AbstractBlock::setVisible (bool isVisible)
{
    mBox->setBorderVisibility (isVisible);
}

bool CSVSettings::AbstractBlock::isVisible () const
{
    return mBox->borderVisibile();
}

QWidget *CSVSettings::AbstractBlock::getParent() const
{
    return mWidgetParent;
}

void CSVSettings::AbstractBlock::slotUpdate (const QString &value)
{
    slotUpdateSetting (objectName(), value);
}

void CSVSettings::AbstractBlock::slotSetEnabled(bool value)
{
    mBox->setEnabled(value);
}

void CSVSettings::AbstractBlock::slotUpdateSetting (const QString &settingName, const QString &settingValue)
{
    bool doEmit = true;
    updateBySignal (settingName, settingValue, doEmit);

    if (doEmit)
        emit signalUpdateSetting (settingName, settingValue);
}
