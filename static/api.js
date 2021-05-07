class api
{
	api_path = '../api/';
	timeout = 1000;

	constructor()
	{
	}

	reboot(fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'reboot',
			{
				type: 'GET',
				crossDomain: true,
				success: fn,
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	get_eth(fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'ethernet',
			{
				type: 'GET',
				crossDomain: true,
				success: fn,
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	get_relay(fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'GET',
				crossDomain: true,
				success: fn,
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	get_relay_state(fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'GET',
				data: {
					"state": "true"
				},
				crossDomain: true,
				success: fn,
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	set_relay_state(relay, state, fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'GET',
				data: {
					"state": state,
					"relay": relay
				},
				crossDomain: true,
				success: fn,
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	get_setting(fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'setting',
			{
				type: 'GET',
				crossDomain: true,
				success: fn,
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	set_setting(data, fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'setting',
			{
				type: 'POST',
				crossDomain: true,
				success: fn,
				data: JSON.stringify(data),
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	set_relay(data, fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'POST',
				crossDomain: true,
				success: fn,
				data: JSON.stringify(data),
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}

	set_eth(data, fn, fn_fail)
	{
		$.ajax(
			this.api_path + 'ethernet',
			{
				type: 'POST',
				crossDomain: true,
				success: fn,
				data: JSON.stringify(data),
				error: fn_fail,
				timeout: this.timeout
			}
		);
	}
}

export default api
