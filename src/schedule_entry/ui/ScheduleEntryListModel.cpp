#include <QIODevice>

#include "Schedule.h"
#include "ScheduleRoles.h"
#include "ScheduleEntryListModel.h"



ScheduleEntryListModel::ScheduleEntryListModel(QObject* parent)
    : QAbstractListModel(parent) {}



int ScheduleEntryListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return _ids.size();
}

QVariant ScheduleEntryListModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if (!_registry || row < 0 || row >= _ids.size())
        return {};

    qulonglong id = _ids[row];
    const ScheduleEntry* entry = _registry->entity(id);
    if (!entry)
        return {};

    switch (role)
    {
    case ScheduleRoles::ScheduleEntryName:
        return entry->name();

    case ScheduleRoles::ScheduleEntryCode:
        return entry->code();

        case ScheduleRoles::ScheduleEntryDetails:
    {
        if (entry->countedAsWork())
        {
            if (entry->startTime() && entry->endTime())
                return entry->startTime()->toString("HH:mm") + " - " + entry->endTime()->toString("HH:mm");
            return QString();
        }
        else
        {
            double rate = entry->paymentRate();
            return (rate == 0.0) ? "Not paid" : QString("%1% paid").arg(rate * 100);
        }
    }

    case Qt::BackgroundRole:
        return entry->colorScheme().background;

    case Qt::ForegroundRole:
        return entry->colorScheme().foreground;

    case Qt::UserRole:
        return entry->id();

    default:
        return {};
    }
}



Qt::ItemFlags ScheduleEntryListModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
    if (index.isValid())
        return defaultFlags | Qt::ItemIsDragEnabled;

    return defaultFlags | Qt::ItemIsDropEnabled;
}

Qt::DropActions ScheduleEntryListModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QStringList ScheduleEntryListModel::mimeTypes() const
{
    return { "application/x-schedule-entry-row" };
}

QMimeData* ScheduleEntryListModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.isEmpty())
        return nullptr;

    QMimeData* mime = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    stream << indexes.first().row();
    mime->setData("application/x-schedule-entry-row", encodedData);
    return mime;
}

bool ScheduleEntryListModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    Q_UNUSED(column);

    if (action != Qt::MoveAction || !data->hasFormat("application/x-schedule-entry-row"))
        return false;

    QByteArray encodedData = data->data("application/x-schedule-entry-row");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    int sourceRow;
    stream >> sourceRow;

    int destinationRow = row;
    if (destinationRow == -1)
        destinationRow = parent.isValid() ? parent.row() : _ids.size();

    int targetIndex = destinationRow;
    if (destinationRow > sourceRow)
        targetIndex--;

    if (sourceRow == targetIndex)
        return false;

    emit scheduleEntryMoveRequested(sourceRow, targetIndex);
    return true;
}



qulonglong ScheduleEntryListModel::getIdAt(int row) const
{
    return _ids.value(row, 0);
}

int ScheduleEntryListModel::getRowById(qulonglong id) const
{
    return _ids.indexOf(id);
}



void ScheduleEntryListModel::onScheduleOpened(const Schedule& schedule)
{
    setRegistry(&schedule.scheduleEntryRegistry());
}

void ScheduleEntryListModel::onScheduleClosed()
{
    setRegistry(nullptr);
}



void ScheduleEntryListModel::onScheduleEntryRegistered(qulonglong id, const ScheduleEntryData&)
{
    int newRow = _ids.size();
    beginInsertRows(QModelIndex(), newRow, newRow);
    _ids.append(id);
    endInsertRows();
}

void ScheduleEntryListModel::onScheduleEntryUnregistered(qulonglong id)
{
    int row = _ids.indexOf(id);
    if (row != -1)
    {
        beginRemoveRows(QModelIndex(), row, row);
        _ids.removeAt(row);
        endRemoveRows();
    }
}

void ScheduleEntryListModel::onScheduleEntryModified(qulonglong id, const ScheduleEntryData&)
{
    int row = _ids.indexOf(id);
    if (row != -1)
    {
        QModelIndex idx = index(row);
        emit dataChanged(idx, idx);
    }
}

void ScheduleEntryListModel::onScheduleEntryMoved(qsizetype from, qsizetype to)
{
    int destChild = (to > from) ? to + 1 : to;
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), destChild);
    _ids.move(from, to);
    endMoveRows();
}



void ScheduleEntryListModel::setRegistry(const ScheduleEntryRegistry* registry)
{
    beginResetModel();
    _registry = registry;
    _ids.clear();
    if (_registry)
        _ids = _registry->ids();
    endResetModel();
}