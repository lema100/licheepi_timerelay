class index
{
	api_path = 'http://localhost:8000/api/';
	setting;
	relay;

	constructor()
	{
		let ctx = this;
		this.get_relay(function (data)
		{
			ctx.relay = data;
			ctx.redraw();
			console.log(data);
		});
		this.get_setting(function (data)
		{
			ctx.setting = data;
			ctx.redraw();
			console.log(data);
		});
	}
	get_relay(fn)
	{
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'GET',
				crossDomain: true,
				success: fn
			}
		);
	}
	get_setting(fn)
	{
		$.ajax(
			this.api_path + 'setting',
			{
				type: 'GET',
				crossDomain: true,
				success: fn
			}
		);
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
		return Object.keys(obj).sort().reduce(function(Obj, key) {
			Obj[key] = obj[key];
			return Obj;
			}, {});
	}
	update_params(ctx, id)
	{
		console.log("updated params " + id);
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
	redraw()
	{
		let _ctx = this;
		let _page = this.get_query("menu");
		let _html = $('#__main_area').empty();
		_html.append($('<br>'));

		if (_page == 'system')
		{
			_html.append($('<h3>').text("System page"));
		}
		else if (_page == "relays")
		{
			for (let key in this.relay["data"])
			{
				let _relay = this.relay["data"][key];
				if (typeof _relay === 'object' && _relay !== null)
				{
					_html.append(JSON.stringify(_relay));
					_html.append($("<h4>", {"text" : "Relay " + key}))
					let div_mode = $("<div>", {"class" : "input-group mb-1 w-100"});
					div_mode.append($("<div>", {"class" : "input-group-prepend w-25"})
						.append($("<div>", {"class" : "input-group-text w-100", "for" : key + "_inputMode", "text" : "Mode"}))
					);
					div_mode.append($("<select>", {"class" : "custom-select", "id" : key + "_inputMode"})
						.append($("<option>", {"value" : "TIME", "text" : "TIME"}))
						.append($("<option>", {"value" : "MANUAL", "text" : "MANUAL"}))
						.append($("<option>", {"value" : "PULSE", "text" : "PULSE"}))
						.append($("<option>", {"value" : "PWM", "text" : "PWM"}))
						.append($("<option>", {"value" : "OFF", "text" : "OFF"}))
						.change(function () {_ctx.update_params(_ctx, this.id);})
						.val(_relay["mode"])
					);
					let div_gpio = $("<div>", {"class" : "input-group mb-1 w-100"});
					div_gpio.append($("<div>", {"class" : "input-group-prepend w-25"})
						.append($("<span>", {"class" : "input-group-text w-100", "text" : "GPIO"}))
					);
					div_gpio.append($("<input>", {
						"id" : key + "_inputGPIO",
						"class" : "form-control",
						"type" : "text",
						"value" : _relay["gpio"]
					}).change(function () {_ctx.update_params(_ctx, this.id);}));

					_html.append(div_mode);
					_html.append(div_gpio);

					switch (_relay["mode"])
					{
						case "MANUAL":
							let _btn_on = $("<button>", {
								"type" : "button",
								"id" : key + "_inputStateOn",
								"class" : "btn btn-outline-success w-25",
								"text" : "ON",
							});
							let _btn_off = $("<button>", {
								"type" : "button",
								"id" : key + "_inputStateOff",
								"class" : "btn btn-outline-danger w-25",
								"text" : "OFF",
							});

							if (_relay["state"] == "OFF")
								_btn_off.addClass("active");
							else
								_btn_on.addClass("active");

							_btn_on.click(function () {_ctx.update_params(_ctx, this.id);});
							_btn_off.click(function () {_ctx.update_params(_ctx, this.id);});

							_html.append(_btn_on);
							_html.append(_btn_off);
							_html.append($("<br>"));
							break;
						case "PWM":
							let div_pulse_on = $("<div>", {"class" : "input-group mb-1 w-100"});
							div_pulse_on.append($("<div>", {"class" : "input-group-prepend w-25"})
								.append($("<span>", {"class" : "input-group-text w-100", "text" : "Pulse ON"}))
							);
							div_pulse_on.append($("<input>", {
								"id" : key + "_inputPulseOn",
								"class" : "form-control",
								"type" : "text",
								"value" : _relay["pulse on"]
							}).change(function () {_ctx.update_params(_ctx, this.id);}));

							let div_pulse_off = $("<div>", {"class" : "input-group mb-1 w-100"});
							div_pulse_off.append($("<div>", {"class" : "input-group-prepend w-25"})
								.append($("<span>", {"class" : "input-group-text w-100", "text" : "Pulse OFF"}))
							);
							div_pulse_off.append($("<input>", {
								"id" : key + "_inputPulseOff",
								"class" : "form-control",
								"type" : "text",
								"value" : _relay["pulse off"]
							}).change(function () {_ctx.update_params(_ctx, this.id);}));

							_html.append(div_pulse_on);
							_html.append(div_pulse_off);
							break;
						case "TIME":
							let _tbl = $("<table>", {"class" : "table table-sm table-bordered w-75"});
							let _thead = $("<thead>", {"class" : "thead-light"})
								.append($("<tr>"))
								.append($("<th>", {"class" : "w-25", "scope" : "col", "text" : "Time"}))
								.append($("<th>", {"class" : "w-25", "scope" : "col", "text" : "State"}))
								.append($("<th>", {"class" : "w-25", "scope" : "col", "text" : "Action"}));
							let _tbody = $("<tbody>");

							for (let time in _relay["timeline"])
							{
								let on_off_select = $("<select>", {"class" : "custom-select", "id" : key + "_inputTimeOnOff_"  + time.replaceAll(":", "-")})
									.append($("<option>", {"value" : "ON", "text" : "ON"}))
									.append($("<option>", {"value" : "OFF", "text" : "OFF"}))
									.change(function () {_ctx.update_params(_ctx, this.id);})
									.val(_relay["timeline"][time]);

								let delete_btn = $("<button>", {
									"type" : "button",
									"id" : key + "_inputTimeDelete_" + time.replaceAll(":", "-"),
									"class" : "btn btn-outline-danger w-50",
									"text" : "DELETE",
								}).click(function () {_ctx.update_params(_ctx, this.id);});

								let append_btn = $("<button>", {
									"type" : "button",
									"id" : key + "_inputTimeAppend_" + time.replaceAll(":", "-"),
									"class" : "btn btn-outline-success w-50",
									"text" : "APPEND",
								}).click(function () {_ctx.update_params(_ctx, this.id);});

								let time_input = $("<input>", {
									"id" : key + "_inputTime_" + time.replaceAll(":", "-"),
									"class" : "form-control",
									"step" : "1",
									"type" : "time",
									"value" : time
								}).change(function () {_ctx.update_params(_ctx, this.id);});

								_tbody.append($("<tr>"))
									.append($("<th>", {"scope" : "row"}).append(time_input))
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
		}
		else
			_html.append($('<h3>').text("Home page"));
	}
}

new index();
