// TODO migrate to Gnome 45 / ESM

import Meta from 'gi://Meta';
import {Extension} from 'resource:///org/gnome/shell/extensions/extension.js';

export default class Maximizer extends Extension {
    _windowCreatedId;

    constructor(metadata) {
        super(metadata);
    }

    enable() {
        this._windowCreatedId = global.display.connect('window-created', (d, win) => {
            // Only try to maximize windows that are marked to support this.
            // Other windows (e.g. dialogs) can often actually be maximized,
            // but then no longer unmaximized by the user, so we really need
            // to check this.
            if (win.can_maximize()) {
                win.maximize(Meta.MaximizeFlags.HORIZONTAL | Meta.MaximizeFlags.VERTICAL);
            } else {
                win.unmaximize(Meta.MaximizeFlags.HORIZONTAL | Meta.MaximizeFlags.VERTICAL);
            }
        });
    }

    disable() {
        if (this._windowCreatedId) {
            global.display.disconnect(this._windowCreatedId);
            this._windowCreatedId = null;
        }
    }

}