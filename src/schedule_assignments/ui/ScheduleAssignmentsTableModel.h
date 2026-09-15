#pragma once

#include <optional>
#include <QAbstractTableModel>
#include <QDate>
#include <QList>
#include <QtTypes>

#include "Schedule.h"
#include "EmployeeData.h"
#include "ScheduleEntryData.h"


class ScheduleAssignmentsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ScheduleAssignmentsTableModel(QObject* parent = nullptr);

    void setSchedule(const Schedule* schedule);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    qulonglong getEmployeeIdFromRow(int row) const;
    QDate getDateFromColumn(int col) const;
    int findRowByEmployeeId(qulonglong id) const;
    int findColByDate(const QDate& date) const;

public slots:
    void onEmployeeRegistered(qulonglong id, const EmployeeData& data);
    void onEmployeeUnregistered(qulonglong id);
    void onEmployeeModified(qulonglong id, const EmployeeData& data);
    void onEmployeeMoved(qsizetype from, qsizetype to);

    void onEntryModified(qulonglong id, const ScheduleEntryData& data);
    void onAssignmentChanged(qulonglong employeeId, const QDate& date, std::optional<qulonglong> scheduleEntryId);

private:
    const Schedule* _schedule = nullptr;
    QList<qulonglong> _employeeIds;
};