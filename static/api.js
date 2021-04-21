class api
{
	api_path = 'http://localhost:8000/api/';
	constructor()
	{
	}
	get_eth(fn)
	{
		$.ajax(
			this.api_path + 'ethernet',
			{
				type: 'GET',
				crossDomain: true,
				success: fn
			}
		);
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
	get_relay_state(fn)
	{
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'GET',
				data: {
					"state" : "true"
				},
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
}

export default api