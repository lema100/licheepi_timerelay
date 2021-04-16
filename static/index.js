class index
{
	api_path = 'http://localhost:8000/api/';
	setting;
	relay;
	constructor()
	{
		var context = this;
		$.ajax(
			this.api_path + 'setting',
			{
				type: 'GET',
				crossDomain: true,
				success: function(data)
				{
					context.setting = data;
					context.redraw();
					console.log(data);
				}
			}
		);
		$.ajax(
			this.api_path + 'relay',
			{
				type: 'GET',
				crossDomain: true,
				success: function(data)
				{
					context.relay = data;
					context.update_menu();
					console.log(data);
				}
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
	update_menu()
	{
		let relays_menu = document.getElementById('__relays_menu');
		relays_menu.innerHTML = JSON.stringify(this.relay);
	}
	redraw()
	{
		let page = this.get_query("menu");
		let main_area = document.getElementById('__main_area');
		if (page == 'home')
		{
			main_area.innerHTML = '123';
		}
		else if (page == 'system')
		{
			main_area.innerHTML = '123123';
		}
	}
}

new index();
