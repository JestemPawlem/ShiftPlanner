#pragma once

#include <QAbstractTableModel>
#include <QDate>
#include <QList>
#include <optional>

#include "Schedule.h"

class ScheduleDemandsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ScheduleDemandsTableModel(QObject* parent = nullptr);

    void setSchedule(const Schedule* schedule);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    qulonglong getScheduleEntryIdFromRow(int row) const;
    QDate getDateFromCol(int col) const;
    int findRowByScheduleEntryId(qulonglong id) const;
    int findColByDate(const QDate& date) const;

public slots:
    void onScheduleEntryRegistered(qulonglong id, const ScheduleEntryData& data);
    void onScheduleEntryUnregistered(qulonglong id);
    void onScheduleEntryModified(qulonglong id, const ScheduleEntryData& data);
    void onScheduleEntryMoved(qsizetype from, qsizetype to);
    void onDemandChanged(qulonglong scheduleEntryId, const QDate& date, qsizetype count);
    void onAssignmentChanged(qulonglong employeeId, const QDate& date, std::optional<qulonglong> scheduleEntryId);

signals:
    void demandChangeRequested(qulonglong entryId, const QDate& date, qsizetype demand);

private:
    void reloadEntryIds();

    const Schedule* _schedule = nullptr;
    QList<qulonglong> _scheduleEntryIds;
};