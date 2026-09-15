#pragma once

#include <QColor>
#include <QVariant>
#include <QVariantMap>


struct ColorScheme
{
    QColor background;
    QColor foreground;

    bool operator==(const ColorScheme& other) const = default;

    QVariant exportState() const
    {
        return QVariantMap{
            { "background", background.name() },
            { "foreground", foreground.name() }
        };
    }

    static ColorScheme importState(const QVariant& data)
    {
        const QVariantMap map = data.toMap();
        return {
            QColor(map["background"].toString()),
            QColor(map["foreground"].toString())
        };
    }
};

Q_DECLARE_METATYPE(ColorScheme)