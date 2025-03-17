#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPointer>
#include <QAbstractListModel>
#include <QSettings>
#include <QJsonArray>
#include <QJsonObject>
#include <QToolButton>
#include <QFileDialog>
#include <QPushButton>

namespace Ui
{
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    QScopedPointer <Ui::SettingsDialog> m_ui;
    QJsonArray engineList;

public:
    SettingsDialog();
    ~SettingsDialog();

public slots:
    void onChooseTriggered();
    void onAddTriggered();
    void onRemoveTriggered();
    void onApplyTriggered();
    void onApplyEnabled();
};

#endif // SETTINGSDIALOG_H
