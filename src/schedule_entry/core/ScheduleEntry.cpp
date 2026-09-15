#include "ScheduleEntry.h"

#include <QVariantMap>


ScheduleEntry::ScheduleEntry(qulonglong id) :
	_id(id)
{}

ScheduleEntry::ScheduleEntry(qulonglong id, const ScheduleEntryData& data) :
	_id(id),
	_code(data.code),
	_name(data.name),
	_colorScheme(data.colorScheme),
	_category(data.category),
	_startTime(data.startTime),
	_endTime(data.endTime),
	_paymentRate(data.paymentRate),
	_isCountedAsWork(data.isCountedAsWork)
{}


bool ScheduleEntry::operator==(const ScheduleEntry& other) const { return _id == other._id; }
bool ScheduleEntry::operator!=(const ScheduleEntry& other) const { return _id != other._id; }


ScheduleEntryData ScheduleEntry::data() const
{
	return { _code, _name, _colorScheme, _category, _startTime, _endTime, _paymentRate, _isCountedAsWork };
}

void ScheduleEntry::setData(const ScheduleEntryData& data)
{
	_code = data.code;
	_name = data.name;
	_colorScheme = data.colorScheme;
	_category = data.category;
	_startTime = data.startTime;
	_endTime = data.endTime;
	_paymentRate = data.paymentRate;
	_isCountedAsWork = data.isCountedAsWork;
}


qulonglong ScheduleEntry::id() const { return _id; }
QString ScheduleEntry::code() const { return _code; }
QString ScheduleEntry::name() const { return _name; }
ColorScheme ScheduleEntry::colorScheme() const { return _colorScheme; }
ScheduleEntryCategory ScheduleEntry::category() const { return _category; }
std::optional<QTime> ScheduleEntry::startTime() const { return _startTime; }
std::optional<QTime> ScheduleEntry::endTime() const { return _endTime; }
double ScheduleEntry::paymentRate() const { return _paymentRate; }
bool ScheduleEntry::countedAsWork() const { return _isCountedAsWork; }

bool ScheduleEntry::isWork() const { return _category == ScheduleEntryCategory::Work; }


QVariant ScheduleEntry::exportState() const
{
	QVariantMap result;

	result["code"] = _code;
	result["name"] = _name;
	result["colorScheme"] = _colorScheme.exportState();
	result["category"] = static_cast<int>(_category);

	if (_startTime)
		result["startTime"] = *_startTime;

	if (_endTime)
		result["endTime"] = *_endTime;

	result["paymentRate"] = _paymentRate;
	result["isCountedAsWork"] = _isCountedAsWork;

	return result;
}

result_t<void> ScheduleEntry::importState(const QVariant& data)
{
	if (!data.canConvert<QVariantMap>())
		return std::unexpected("Illegal schedule entry type data format");

	QVariantMap map = data.toMap();

	_code = map["code"].toString();
	_name = map["name"].toString();
	_colorScheme = ColorScheme::importState(map["colorScheme"]);
	_category = static_cast<ScheduleEntryCategory>(map["category"].toInt());

	if (map.contains("startTime") && !map["startTime"].isNull())
		_startTime = map["startTime"].toTime();
	else
		_startTime = std::nullopt;

	if (map.contains("endTime") && !map["endTime"].isNull())
		_endTime = map["endTime"].toTime();
	else
		_endTime = std::nullopt;

	_paymentRate = map["paymentRate"].toDouble();
	_isCountedAsWork = map["isCountedAsWork"].toBool();

	return {};
}