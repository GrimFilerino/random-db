package main

import (
    "github.com/gofiber/fiber/v2"
	"main/server"
)

func main() {
	app := fiber.New();
	
	data_base := app.Group("/data-base");
	server.data_base_route(data_base);

	app.Listen(":3000");
}

