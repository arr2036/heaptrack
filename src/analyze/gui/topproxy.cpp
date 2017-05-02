/*
 * Copyright 2016-2017 Milian Wolff <mail@milianw.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "topproxy.h"

#include <KLocalizedString>

namespace {
TreeModel::Columns toSource(TopProxy::Type type)
{
    switch (type) {
    case TopProxy::Peak:
        return TreeModel::PeakColumn;
    case TopProxy::Leaked:
        return TreeModel::LeakedColumn;
    case TopProxy::Allocations:
        return TreeModel::AllocationsColumn;
    case TopProxy::Temporary:
        return TreeModel::TemporaryColumn;
    case TopProxy::Allocated:
        return TreeModel::AllocatedColumn;
    }
    Q_UNREACHABLE();
}
}

TopProxy::TopProxy(Type type, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_type(type)
{
}

TopProxy::~TopProxy() = default;

bool TopProxy::filterAcceptsColumn(int source_column, const QModelIndex& /*source_parent*/) const
{
    return source_column == TreeModel::LocationColumn || source_column == toSource(m_type);
}

bool TopProxy::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    if (source_parent.isValid()) {
        // only show top rows
        return false;
    }
    if (!sourceModel()->index(source_row, toSource(m_type)).data(TreeModel::SortRole).toULongLong()) {
        // don't show rows that didn't leak anything, or didn't trigger any
        // temporary allocations
        return false;
    }
    return true;
}
