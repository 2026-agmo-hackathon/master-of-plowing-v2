"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (Object.prototype.hasOwnProperty.call(b, p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        if (typeof b !== "function" && b !== null)
            throw new TypeError("Class extends value " + String(b) + " is not a constructor or null");
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g = Object.create((typeof Iterator === "function" ? Iterator : Object).prototype);
    return g.next = verb(0), g["throw"] = verb(1), g["return"] = verb(2), typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (g && (g = 0, op[0] && (_ = 0)), _) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
Object.defineProperty(exports, "__esModule", { value: true });
var test_1 = require("@playwright/test");
(0, test_1.test)('run controls stack at 320px and adapt at tablet/desktop widths', function (_a) { return __awaiter(void 0, [_a], void 0, function (_b) {
    var _i, _c, width, actions, buttons, first, second, setup;
    var page = _b.page;
    return __generator(this, function (_d) {
        switch (_d.label) {
            case 0:
                _i = 0, _c = [320, 768, 1280];
                _d.label = 1;
            case 1:
                if (!(_i < _c.length)) return [3 /*break*/, 10];
                width = _c[_i];
                return [4 /*yield*/, page.setViewportSize({ width: width, height: 900 })];
            case 2:
                _d.sent();
                return [4 /*yield*/, page.goto('/')];
            case 3:
                _d.sent();
                actions = page.getByTestId('run-actions');
                return [4 /*yield*/, (0, test_1.expect)(actions).toBeVisible()];
            case 4:
                _d.sent();
                buttons = actions.locator('button');
                return [4 /*yield*/, (0, test_1.expect)(buttons).toHaveCount(2)];
            case 5:
                _d.sent();
                return [4 /*yield*/, buttons.nth(0).boundingBox()];
            case 6:
                first = _d.sent();
                return [4 /*yield*/, buttons.nth(1).boundingBox()];
            case 7:
                second = _d.sent();
                (0, test_1.expect)(first).not.toBeNull();
                (0, test_1.expect)(second).not.toBeNull();
                if (width === 320) {
                    (0, test_1.expect)(Math.abs(first.x - second.x)).toBeLessThan(2);
                    (0, test_1.expect)(second.y).toBeGreaterThan(first.y + first.height - 2);
                }
                else {
                    (0, test_1.expect)(second.x).toBeGreaterThan(first.x + first.width - 2);
                    (0, test_1.expect)(Math.abs(first.y - second.y)).toBeLessThan(2);
                }
                return [4 /*yield*/, page.getByTestId('run-fields').boundingBox()];
            case 8:
                setup = _d.sent();
                (0, test_1.expect)(setup).not.toBeNull();
                (0, test_1.expect)(setup.x).toBeGreaterThanOrEqual(0);
                (0, test_1.expect)(setup.x + setup.width).toBeLessThanOrEqual(width);
                _d.label = 9;
            case 9:
                _i++;
                return [3 /*break*/, 1];
            case 10: return [2 /*return*/];
        }
    });
}); });
(0, test_1.test)('team input preserves focus and IME text without simulator setup messages', function (_a) { return __awaiter(void 0, [_a], void 0, function (_b) {
    var input, _c, setupMessages, start;
    var page = _b.page;
    return __generator(this, function (_d) {
        switch (_d.label) {
            case 0: return [4 /*yield*/, page.route('**/get_assigned_ports', function (route) { return route.fulfill({
                    status: 200, contentType: 'application/json', body: '{"app":1456}',
                }); })];
            case 1:
                _d.sent();
                return [4 /*yield*/, page.addInitScript(function () {
                        var sent = [];
                        globalThis.__sent = sent;
                        var FakeWebSocket = /** @class */ (function (_super) {
                            __extends(FakeWebSocket, _super);
                            function FakeWebSocket() {
                                var _this = _super.call(this) || this;
                                _this.readyState = 1;
                                setTimeout(function () {
                                    _this.dispatchEvent(new Event('open'));
                                    _this.dispatchEvent(new MessageEvent('message', { data: JSON.stringify({
                                            kind: 'sim_setup_state', phase: 'idle', busy: false,
                                            maps: [{ id: 'map-a', label: 'Map A' }],
                                            tractors: [{ id: 'tractor-a', label: 'Tractor A' }],
                                            implements: [{ id: 'none', label: 'None', widthM: 0 }],
                                            selection: { mapId: 'map-a', tractorId: 'tractor-a', implementId: 'none',
                                                stopped: true, live: true, snapshotAgeMs: 10 },
                                            confirmedSetup: { mapId: 'map-a', tractorId: 'tractor-a', implementId: 'none' },
                                        }) }));
                                    _this.dispatchEvent(new MessageEvent('message', { data: JSON.stringify({
                                            kind: 'pp_telemetry', runState: 'idle', dtMs: 100,
                                        }) }));
                                }, 0);
                                return _this;
                            }
                            FakeWebSocket.prototype.send = function (value) { sent.push(value); };
                            FakeWebSocket.prototype.close = function () { this.readyState = 3; this.dispatchEvent(new Event('close')); };
                            FakeWebSocket.OPEN = 1;
                            FakeWebSocket.CLOSED = 3;
                            return FakeWebSocket;
                        }(EventTarget));
                        Object.defineProperty(globalThis, 'WebSocket', { value: FakeWebSocket });
                    })];
            case 2:
                _d.sent();
                return [4 /*yield*/, page.goto('/')];
            case 3:
                _d.sent();
                input = page.getByLabel('Team name');
                return [4 /*yield*/, (0, test_1.expect)(input).toBeEnabled()];
            case 4:
                _d.sent();
                return [4 /*yield*/, input.focus()];
            case 5:
                _d.sent();
                return [4 /*yield*/, input.pressSequentially('밭갈이팀')];
            case 6:
                _d.sent();
                return [4 /*yield*/, (0, test_1.expect)(input).toBeFocused()];
            case 7:
                _d.sent();
                return [4 /*yield*/, (0, test_1.expect)(input).toHaveValue('밭갈이팀')];
            case 8:
                _d.sent();
                return [4 /*yield*/, input.evaluate(function (element) {
                        var C = globalThis.CompositionEvent;
                        element.dispatchEvent(new C('compositionstart', { data: '한' }));
                        element.dispatchEvent(new C('compositionend', { data: '한' }));
                    })];
            case 9:
                _d.sent();
                return [4 /*yield*/, (0, test_1.expect)(input).toBeFocused()];
            case 10:
                _d.sent();
                _c = test_1.expect;
                return [4 /*yield*/, page.evaluate(function () { return localStorage.getItem('master_of_plow_teamName'); })];
            case 11:
                _c.apply(void 0, [_d.sent()])
                    .toBe('밭갈이팀');
                return [4 /*yield*/, page.evaluate(function () {
                        return globalThis.__sent
                            .map(function (raw) { return JSON.parse(raw); })
                            .filter(function (message) { return message.kind === 'set_sim_setup'; });
                    })];
            case 12:
                setupMessages = _d.sent();
                (0, test_1.expect)(setupMessages).toEqual([]);
                page.on('dialog', function (dialog) { return void dialog.accept(); });
                return [4 /*yield*/, page.getByRole('button', { name: 'Start' }).click()];
            case 13:
                _d.sent();
                return [4 /*yield*/, page.evaluate(function () {
                        return globalThis.__sent
                            .map(function (raw) { return JSON.parse(raw); })
                            .find(function (message) { return message.kind === 'start_run'; });
                    })];
            case 14:
                start = _d.sent();
                (0, test_1.expect)(start).toMatchObject({ teamName: '밭갈이팀', implementId: 'none' });
                return [2 /*return*/];
        }
    });
}); });
