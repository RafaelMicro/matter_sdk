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

import {v4 as uuidv4} from 'uuid';

import {Actions} from '../../common/actions';
import {Engine} from '../../common/engine';
import {
  generateSqlWithInternalLayout,
} from '../../common/internal_layout_utils';
import {
  PrimaryTrackSortKey,
  SCROLLING_TRACK_GROUP,
  Selection,
} from '../../common/state';
import {OnSliceClickArgs} from '../../frontend/base_slice_track';
import {globals} from '../../frontend/globals';
import {
  NamedSliceTrack,
  NamedSliceTrackTypes,
} from '../../frontend/named_slice_track';
import {NewTrackArgs, Track} from '../../frontend/track';
import {DecideTracksResult} from '../chrome_scroll_jank';
import {GenericSliceDetailsTab, Columns} from '../../frontend/generic_slice_details_tab';

export {Data} from '../chrome_slices';

export class TopLevelScrollTrack extends NamedSliceTrack {
  static readonly kind = 'org.chromium.TopLevelScrolls.scrolls';

  static create(args: NewTrackArgs): Track {
    return new TopLevelScrollTrack(args);
  }

  constructor(args: NewTrackArgs) {
    super(args);
  }

  async initSqlTable(tableName: string) {
    const sql =
        `CREATE VIEW ${tableName} AS ` + generateSqlWithInternalLayout({
          columns: [`printf("Scroll %s", CAST(id AS STRING)) AS name`, '*'],
          layoutParams: {ts: 'ts', dur: 'dur'},
          sourceTable: 'chrome_scrolls',
          orderByClause: 'ts',
        });
    await this.engine.query(sql);
  }

  isSelectionHandled(selection: Selection) {
    if (selection.kind !== 'BASIC_SQL_OBJECT') {
      return false;
    }
    return selection.trackId === this.trackId;
  }

  onSliceClick(args: OnSliceClickArgs<NamedSliceTrackTypes['slice']>) {
    const columns : Columns = {};
    columns['id'] = {displayName: 'Scroll Id (gesture_scroll_id)'};
    columns['ts'] = {displayName: 'Start Time'};
    columns['dur'] = {displayName: 'Duration'};

    const title = 'Current Chrome Scroll';

    globals.dispatch(Actions.selectBasicSqlSlice({
      id: args.slice.id,
      sqlTableName: this.tableName,
      start: args.slice.start,
      duration: args.slice.duration,
      trackId: this.trackId,
      detailsPanelConfig: {
        kind: GenericSliceDetailsTab.kind,
        config: {
          id: args.slice.id,
          sqlTableName: this.tableName,
          title: title,
          columns: columns,
        },
      },
    }));
  }
}

export async function addTopLevelScrollTrack(engine: Engine):
    Promise<DecideTracksResult> {
  const result: DecideTracksResult = {
    tracksToAdd: [],
  };

  await engine.query(`SELECT IMPORT('chrome.chrome_scrolls');`);

  result.tracksToAdd.push({
    id: uuidv4(),
    engineId: engine.id,
    kind: TopLevelScrollTrack.kind,
    trackSortKey: PrimaryTrackSortKey.ASYNC_SLICE_TRACK,
    name: 'Top Level Scrolls',
    config: {},
    trackGroup: SCROLLING_TRACK_GROUP,
  });

  return result;
}
