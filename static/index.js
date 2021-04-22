import api from './api.js'

class index
{
	relay_state;
	ethernet;
	setting;
	relay;
	page;
	api;

	constructor()
	{
		this.api = new api();
		this.page = this.get_query("menu");
		let ctx = this;

		this.api.get_eth(function (data)
		{
			ctx.ethernet = data;
			ctx.redraw();
			console.log(data);
		}, this.api_fail_fn("FAIL: Request ethernet settings.", ctx));

		this.api.get_relay(function (data)
		{
			ctx.relay = data;
			ctx.redraw();
			console.log(data);
		}, this.api_fail_fn("FAIL: Request relay settings.", ctx));

		this.api.get_relay_state(function (data)
		{
			ctx.relay_state = data;
			ctx.redraw();
			console.log(data);
		}, this.api_fail_fn("FAIL: Request relay states.", ctx));

		this.api.get_setting(function (data)
		{
			ctx.setting = data;
			ctx.redraw();
			console.log(data);
		}, this.api_fail_fn("FAIL: Request general setting.", ctx));

		$("#btnApply").click(function ()
		{
			ctx.on_clickApply(ctx);
		});
		$("#btnReboot").click(function ()
		{
			ctx.on_clickReboot(ctx);
		});
		$("#splashAlert").hide();

		if (ctx.page != "system" && ctx.page != "relays")
		{
			window.setInterval(function ()
			{
				$("#btnApply").click();
			}, 500);
		}
	}

	api_fail_fn(message, ctx)
	{
		return function (xhr, textStatus, errorThrown)
		{
			ctx.alert("alert alert-danger", message + " Error: " + xhr.statusText);
		};
	}

	alert(_class, _message)
	{
		let alert = $("#splashAlert");
		$("#splashAlertDiv").empty().text(_message);
		alert.attr("class", _class);
		alert.slideDown(500, function ()
		{
			setTimeout(function ()
			{
				alert.slideUp(500, function ()
				{
				});
			}, 2000);
		});
	}

	on_clickApply(ctx)
	{
		console.log("button apply clicked");
		switch (ctx.page)
		{
		case "system":
			ctx.api.set_setting(ctx.setting["data"], function (data)
			{
				ctx.alert("alert alert-success", "INFO: Apply success.");
			}, ctx.api_fail_fn("FAIL: Apply setting.", ctx));
			ctx.api.set_eth(ctx.ethernet["data"], function (data)
			{
				ctx.alert("alert alert-success", "INFO: Apply success.");
			}, ctx.api_fail_fn("FAIL: Apply ethernet.", ctx));
			break;
		case "relays":
			ctx.api.set_relay(ctx.relay["data"], function (data)
			{
				ctx.alert("alert alert-success", "INFO: Apply success.");
			}, ctx.api_fail_fn("FAIL: Apply relay.", ctx));
			break;
		default:
			ctx.api.get_relay_state(function (data)
			{
				ctx.relay_state = data;
				ctx.redraw();
				console.log(data);
			}, ctx.api_fail_fn("FAIL: Request relay states.", ctx));
			ctx.api.get_setting(function (data)
			{
				ctx.setting = data;
				ctx.redraw();
				console.log(data);
			}, ctx.api_fail_fn("FAIL: Request general setting.", ctx));
			break;
		}
	}

	on_clickReboot(ctx)
	{
		console.log("button reboot clicked");
		ctx.api.reboot(function (data)
		{
			ctx.alert("alert alert-danger", "INFO: Reboot success. It may take few minutes.")
		}, ctx.api_fail_fn("FAIL: Reboot.", ctx));
	}

	get_query(name, url = window.location.href)
	{
		name = name.replace(/[\[\]]/g, '\\$&');
		let regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'), results = regex.exec(url);
		if (!results)
			return null;
		if (!results[2])
			return '';
		return decodeURIComponent(results[2].replace(/\+/g, ' '));
	}

	sort(obj)
	{
		return Object.keys(obj).sort().reduce(function (Obj, key)
		{
			Obj[key] = obj[key];
			return Obj;
		}, {});
	}

	update_params(ctx, id)
	{
		console.log("Updated params from id:" + id);
		let id_split = id.replaceAll("-", ":").split("_");
		let elem = $("#" + id);
		let _relay = ctx.relay["data"][id_split[0]];
		if (typeof _relay === 'object' && _relay !== null)
		{
			if (id.includes("inputMode"))
				_relay["mode"] = elem.val();
			if (id.includes("inputGPIO"))
			{
				let val = parseInt(elem.val())
				if (!isNaN(val))
					_relay["gpio"] = val;
			}
			switch (_relay["mode"])
			{
			case "MANUAL":
				if (id.includes("inputState"))
				{
					if (id.includes("On"))
						_relay["state"] = "ON";
					else
						_relay["state"] = "OFF";
				}
				break;
			case "PWM":
				let val = parseInt(elem.val())
				if (!isNaN(val) && id.includes("inputPulseOn"))
					_relay["pulse on"] = val;
				if (!isNaN(val) && id.includes("inputPulseOff"))
					_relay["pulse off"] = val;
				break;
			case "TIME":
				switch (id_split[1])
				{
				case "inputTimeDelete":
					if (Object.keys(_relay["timeline"]).length > 1)
						delete _relay["timeline"][id_split[2]];
					break;
				case "inputTimeAppend":
					if (id_split[2].length == 8)
					{
						_relay["timeline"][id_split[2] + "0"] = _relay["timeline"][id_split[2]];
						_relay["timeline"] = ctx.sort(_relay["timeline"]);
					}
					break;
				case "inputTime":
					_relay["timeline"][elem.val()] = _relay["timeline"][id_split[2]];
					delete _relay["timeline"][id_split[2]];
					_relay["timeline"] = ctx.sort(_relay["timeline"]);
					break;
				case "inputTimeOnOff":
					_relay["timeline"][id_split[2]] = elem.val();
					break;
				default:
					break;
				}
				break;
			case "PULSE":
			case "OFF":
			default:
				break;
			}
		}
		ctx.redraw();
	}

	update_setting(ctx, id)
	{
		console.log("Updated params from id:" + id);
		let ip_patt = /\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b/;

		let sett = this.setting["data"];
		let eth = this.ethernet["data"];
		let elem = $("#" + id);
		let val_int = parseInt(elem.val())
		let ip_test = ip_patt.test(elem.val());

		switch (id)
		{
		case "inputGPIO1":
			if (!isNaN(val_int))
				sett["GPIO green led"] = val_int;
			break;
		case "inputGPIO2":
			if (!isNaN(val_int))
				sett["GPIO red led"] = val_int;
			break;
		case "inputHTTPport":
			if (!isNaN(val_int))
				sett["HTTP port"] = val_int;
			break;
		case "inputRelayCount":
			if (!isNaN(val_int))
				sett["Relay count"] = val_int;
			break;
		case "inputModeTZ":
			sett["Timezone"] = elem.val();
			break;
		case "inputIP":
			if (ip_test)
				eth["IP"] = elem.val();
			break;
		case "inputMASK":
			if (ip_test)
				eth["MASK"] = elem.val();
			break;
		case "inputGATEWAY":
			if (ip_test)
				eth["GATEWAY"] = elem.val();
			break;
		case "inlineRadioDHCP":
			eth["DHCP"] = true;
			break;
		case "inlineRadioStatic":
			eth["DHCP"] = false;
			break;
		default:
			break;
		}
		ctx.redraw();
	}

	redraw()
	{
		let _ctx = this;
		let _html = $('#__main_area').empty();
		_html.append($('<br>'));

		switch (this.page)
		{
		case 'system':
		{
			if (typeof this.setting !== 'object' || this.setting === null)
				return;
			if (typeof this.ethernet !== 'object' || this.ethernet === null)
				return;
			let _sett = this.setting["data"];
			let _eth = this.ethernet["data"];
			if (typeof _sett === 'object' && _sett !== null)
			{
				_html.append($('<h4>').text("System"));
				console.log(JSON.stringify(this.setting));

				let inputs = [
					["GPIO LED 1", "inputGPIO1", "GPIO green led"],
					["GPIO LED 2", "inputGPIO2", "GPIO red led"],
					["HTTP port", "inputHTTPport", "HTTP port"],
					["Relay count", "inputRelayCount", "Relay count"],
//					["Timezone", "inputTimezone", "Timezone"],
				];

				for (let i of inputs)
				{
					let div = $("<div>", {"class": "input-group mb-1 w-50"});
					div.append($("<div>", {"class": "input-group-prepend w-50"})
						.append($("<span>", {"class": "input-group-text w-100", "text": i[0]}))
					);
					div.append($("<input>", {
						"id": i[1],
						"class": "form-control",
						"type": "text",
						"value": _sett[i[2]]
					}).change(function ()
					{
						_ctx.update_setting(_ctx, this.id);
					}));
					_html.append(div);
				}

				let div_TZ = $("<div>", {"class": "input-group mb-1 w-50"});
				div_TZ.append($("<div>", {"class": "input-group-prepend w-50"})
					.append($("<div>", {
						"class": "input-group-text w-100",
						"for": "inputModeTZ",
						"text": "Timezone"
					}))
				);
				let sel_TZ = ($("<select>", {"class": "custom-select", "id": "inputModeTZ"}));
				for (let i of _sett["Timezone list"])
					sel_TZ.append($("<option>", {"value": i, "text": i}));
				sel_TZ.change(function ()
				{
					_ctx.update_setting(_ctx, this.id);
				}).val(_sett["Timezone"]);

				div_TZ.append(sel_TZ);
				_html.append(div_TZ);
				_html.append($("<br>"));
			}

			if (typeof _eth === 'object' && _eth !== null)
			{
				_html.append($('<h4>').text("Ethernet"));
				console.log(JSON.stringify(this.ethernet));

				let div_dhcp = $("<div>", {"class": "form-check form-check-inline w-25"});
				div_dhcp.append($("<input>", {
					"class": "form-check-input",
					"type": "radio",
					"name": "inlineRadioOptions",
					"id": "inlineRadioDHCP",
					"value": true
				}).change(function ()
				{
					_ctx.update_setting(_ctx, this.id);
				}));
				div_dhcp.append($("<label>", {
					"class": "form-check-label",
					"for": "inlineRadioDHCP",
					"text": "DHCP"
				}));
				let div_static = $("<div>", {"class": "form-check form-check-inline w-25"});
				div_static.append($("<input>", {
					"class": "form-check-input",
					"type": "radio",
					"name": "inlineRadioOptions",
					"id": "inlineRadioStatic",
					"value": false
				}).change(function ()
				{
					_ctx.update_setting(_ctx, this.id);
				}));
				div_static.append($("<label>", {
					"class": "form-check-label",
					"for": "inlineRadioStatic",
					"text": "STATIC"
				}));
				_html.append(div_dhcp);
				_html.append(div_static);

				if (_eth["DHCP"])
					$("#inlineRadioDHCP").attr("checked", "checked");
				else
					$("#inlineRadioStatic").attr("checked", "checked");

				let inputs_eth = [
					["IP", "inputIP", "IP"],
					["MASK", "inputMASK", "MASK"],
					["GATEWAY", "inputGATEWAY", "GATEWAY"]
				];

				for (let i of inputs_eth)
				{
					let div = $("<div>", {"class": "input-group mb-1 w-50"});
					div.append($("<div>", {"class": "input-group-prepend w-50"})
						.append($("<span>", {"class": "input-group-text w-100", "text": i[0]}))
					);
					div.append($("<input>", {
						"id": i[1],
						"class": "form-control",
						"type": "text",
						"value": _eth[i[2]],
						"disabled": _eth["DHCP"]
					}).change(function ()
					{
						_ctx.update_setting(_ctx, this.id);
					}));
					_html.append(div);
				}
				_html.append($("<br>"));
				_html.append($('<p>', {
					"class": "text-danger",
					"text": "Note: Timezone, HTTP port and Ethernet setting take effect only after APPLY and REBOOT"
				}))
			}
			break;
		}
		case "relays":
		{
			if (typeof this.relay !== 'object' || this.relay === null)
				return;

			for (let key in this.relay["data"])
			{
				let _relay = this.relay["data"][key];
				if (typeof _relay === 'object' && _relay !== null)
				{
					console.log(JSON.stringify(_relay));
					_html.append($("<h4>", {"text": "Relay " + key}))
					let div_mode = $("<div>", {"class": "input-group mb-1 w-50"});
					div_mode.append($("<div>", {"class": "input-group-prepend w-50"})
						.append($("<div>", {
							"class": "input-group-text w-100",
							"for": key + "_inputMode",
							"text": "Mode"
						}))
					);
					div_mode.append($("<select>", {"class": "custom-select", "id": key + "_inputMode"})
						.append($("<option>", {"value": "TIME", "text": "TIME"}))
						.append($("<option>", {"value": "MANUAL", "text": "MANUAL"}))
//						.append($("<option>", {"value" : "PULSE", "text" : "PULSE"}))
						.append($("<option>", {"value": "PWM", "text": "PWM"}))
						.append($("<option>", {"value": "OFF", "text": "OFF"}))
						.change(function ()
						{
							_ctx.update_params(_ctx, this.id);
						})
						.val(_relay["mode"])
					);
					let div_gpio = $("<div>", {"class": "input-group mb-1 w-50"});
					div_gpio.append($("<div>", {"class": "input-group-prepend w-50"})
						.append($("<span>", {"class": "input-group-text w-100", "text": "GPIO"}))
					);
					div_gpio.append($("<input>", {
						"id": key + "_inputGPIO",
						"class": "form-control",
						"type": "text",
						"value": _relay["gpio"]
					}).change(function ()
					{
						_ctx.update_params(_ctx, this.id);
					}));

					_html.append(div_mode);
					_html.append(div_gpio);

					switch (_relay["mode"])
					{
					case "MANUAL":
						let _btn_on = $("<button>", {
							"type": "button",
							"id": key + "_inputStateOn",
							"class": "btn btn-outline-success w-25",
							"text": "ON",
						});
						let _btn_off = $("<button>", {
							"type": "button",
							"id": key + "_inputStateOff",
							"class": "btn btn-outline-danger w-25",
							"text": "OFF",
						});

						if (_relay["state"] == "OFF")
							_btn_off.addClass("active");
						else
							_btn_on.addClass("active");

						_btn_on.click(function ()
						{
							_ctx.update_params(_ctx, this.id);
							_ctx.api.set_relay_state(key, "ON", {}, _ctx.api_fail_fn("FAIL: Set ON relay " + key + ".", _ctx));
						});
						_btn_off.click(function ()
						{
							_ctx.update_params(_ctx, this.id);
							_ctx.api.set_relay_state(key, "OFF", {}, _ctx.api_fail_fn("FAIL: Set OFF relay " + key + ".", _ctx));
						});

						_html.append(_btn_on);
						_html.append(_btn_off);
						_html.append($("<br>"));
						break;
					case "PWM":
						let div_pulse_on = $("<div>", {"class": "input-group mb-1 w-50"});
						div_pulse_on.append($("<div>", {"class": "input-group-prepend w-50"})
							.append($("<span>", {"class": "input-group-text w-100", "text": "Pulse ON"}))
						);
						div_pulse_on.append($("<input>", {
							"id": key + "_inputPulseOn",
							"class": "form-control",
							"type": "text",
							"value": _relay["pulse on"]
						}).change(function ()
						{
							_ctx.update_params(_ctx, this.id);
						}));

						let div_pulse_off = $("<div>", {"class": "input-group mb-1 w-50"});
						div_pulse_off.append($("<div>", {"class": "input-group-prepend w-50"})
							.append($("<span>", {"class": "input-group-text w-100", "text": "Pulse OFF"}))
						);
						div_pulse_off.append($("<input>", {
							"id": key + "_inputPulseOff",
							"class": "form-control",
							"type": "text",
							"value": _relay["pulse off"]
						}).change(function ()
						{
							_ctx.update_params(_ctx, this.id);
						}));

						_html.append(div_pulse_on);
						_html.append(div_pulse_off);
						break;
					case "TIME":
						let _tbl = $("<table>", {"class": "table table-sm table-bordered w-75"});
						let _thead = $("<thead>", {"class": "thead-light"})
							.append($("<tr>"))
							.append($("<th>", {"class": "w-25", "scope": "col", "text": "Time"}))
							.append($("<th>", {"class": "w-25", "scope": "col", "text": "State"}))
							.append($("<th>", {"class": "w-25", "scope": "col", "text": "Action"}));
						let _tbody = $("<tbody>");

						for (let time in _relay["timeline"])
						{
							let on_off_select = $("<select>", {
								"class": "custom-select",
								"id": key + "_inputTimeOnOff_" + time.replaceAll(":", "-")
							})
								.append($("<option>", {"value": "ON", "text": "ON"}))
								.append($("<option>", {"value": "OFF", "text": "OFF"}))
								.change(function ()
								{
									_ctx.update_params(_ctx, this.id);
								})
								.val(_relay["timeline"][time]);

							let delete_btn = $("<button>", {
								"type": "button",
								"id": key + "_inputTimeDelete_" + time.replaceAll(":", "-"),
								"class": "btn btn-outline-danger w-50 text-truncate",
								"text": "DELETE",
							}).click(function ()
							{
								_ctx.update_params(_ctx, this.id);
							});

							let append_btn = $("<button>", {
								"type": "button",
								"id": key + "_inputTimeAppend_" + time.replaceAll(":", "-"),
								"class": "btn btn-outline-success w-50 text-truncate",
								"text": "APPEND",
							}).click(function ()
							{
								_ctx.update_params(_ctx, this.id);
							});

							let time_input = $("<input>", {
								"id": key + "_inputTime_" + time.replaceAll(":", "-"),
								"class": "form-control",
								"step": "1",
								"type": "time",
								"value": time
							}).change(function ()
							{
								_ctx.update_params(_ctx, this.id);
							});

							_tbody.append($("<tr>"))
								.append($("<th>", {"scope": "row"}).append(time_input))
								.append($("<td>").append(on_off_select))
								.append($("<td>").append(delete_btn).append(append_btn));
						}
						_tbl.append(_thead).append(_tbody);
						_html.append(_tbl);
						break;
					case "PULSE":
					case "OFF":
					default:
						break;
					}
					_html.append($("<br>"));
				}
				else
				{
					_html.append($('<h3>').text("API error."));
					_html.append($('<br>'));
					_html.append($('<h3>').text("Contact technical support."));
				}
			}
			break;
		}
		default:
		{
			if (typeof this.relay_state !== 'object' || this.relay_state === null)
				return;
			if (typeof this.setting !== 'object' || this.setting === null)
				return;

			let _relay_state = this.relay_state["data"];
			let _sett = this.setting["data"];

			_html.append($('<h4>').text("Home"));

			let div = $("<div>", {"class": "input-group mb-1 w-50"});
			div.append($("<div>", {"class": "input-group-prepend w-50"})
				.append($("<span>", {"class": "input-group-text w-100", "text": "Date/Time UTC"}))
			);
			div.append($("<input>", {
				"id": "TimeFateUTC",
				"class": "form-control",
				"type": "text",
				"value": _sett["Current time UTC"],
				"disabled": true
			}));
			_html.append(div);

			for (let i in _relay_state)
			{
				let div = $("<div>", {"class": "input-group mb-1 w-50"});
				div.append($("<div>", {"class": "input-group-prepend w-50"})
					.append($("<span>", {"class": "input-group-text w-100", "text": "Relay " + i}))
				);
				div.append($("<input>", {
					"id": "relayState" + i,
					"class": "form-control" + (_relay_state[i] == "ON" ? " text-success" : " text-danger"),
					"type": "text",
					"value": _relay_state[i],
					"disabled": true
				}));
				_html.append(div);
			}
			break;
		}
		}
	}
}

new index();
