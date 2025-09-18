package server

import (
    "github.com/gofiber/fiber/v2"
)


func dataBaseRoute(group *fiber.Group) {

	group.Get("/:collection/:id", get_document_by_id);

}

func get_document_by_id(c *fiber.Ctx) error {
	collection_name := c.Params("collection");
	id := c.Params("id");



	return nil;
}
