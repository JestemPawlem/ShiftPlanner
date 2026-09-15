#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QMap>


class Schedule;

class ScheduleDemandsWeeklyTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ScheduleDemandsWeeklyTableModel(const Schedule* schedule, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    const QMap<qulonglong, QList<qsizetype>>& templateGrid() const;
    const QList<qulonglong>& entryIds() const;

private:
    const Schedule* _schedule;
    QList<qulonglong> _entryIds;
    QMap<qulonglong, QList<qsizetype>> _grid;
};