// Copyright (C) 2023 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import {SortDirection} from '../common/state';
import {ColumnType} from '../common/query_result';

interface OrderClause {
  fieldName: string;
  direction?: SortDirection;
}

// Interface for defining constraints which can be passed to a SQL query.
export interface SQLConstraints {
  filters?: (undefined|string)[];
  orderBy?: (undefined|string|OrderClause)[];
  groupBy?: (undefined|string)[];
  limit?: number;
}

function isDefined<T>(t: T|undefined): t is T {
  return t !== undefined;
}

// Formatting given constraints into a string which can be injected into
// SQL query.
export function constraintsToQueryFragment(c: SQLConstraints): string {
  const result: string[] = [];

  if (c.filters) {
    const filters = c.filters.filter(isDefined);
    if (filters.length > 0) {
      result.push(`WHERE ${c.filters.join(' and ')}`);
    }
  }
  if (c.groupBy) {
    const groupBy = c.groupBy.filter(isDefined);
    if (groupBy.length > 0) {
      const groups = groupBy.join(', ');
      result.push(`GROUP BY ${groups}`);
    }
  }
  if (c.orderBy) {
    const orderBy = c.orderBy.filter(isDefined);
    if (orderBy.length > 0) {
      const orderBys = orderBy.map((clause) => {
        if (typeof clause === 'string') {
          return clause;
        } else {
          const direction = clause.direction ? ` ${clause.direction}` : '';
          return `${clause.fieldName}${direction}`;
        }
      });
      result.push(`ORDER BY ${orderBys.join(', ')}`);
    }
  }
  if (c.limit) {
    result.push(`LIMIT ${c.limit}`);
  }
  return result.join('\n');
}

// Trace Processor returns number | null for NUM_NULL, while most of the UI
// code uses number | undefined. This functions provides a short-hand
// conversion.
// TODO(altimin): Support NUM_UNDEFINED as a first-class citizen.
export function fromNumNull(n: number|null): number|undefined {
  if (n === null) {
    return undefined;
  }
  return n;
}

export function sqlValueToString(val: ColumnType): string {
  if (val instanceof Uint8Array) {
    return `<blob length=${val.length}>`;
  }
  if (val === null) {
    return 'NULL';
  }
  return val.toString();
}
