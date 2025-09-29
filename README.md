# Project Title

A brief description of what this project does and who it's for



## Database API Documentation

All requests require an `Authorization` header with a Bearer token:

```
Authorization: Bearer ${token}
```

Replace `${token}` with your actual API token.

---

#### Grep documents in collection

```http
GET /query/${collection}?limit=${limit}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `limit`       | `int`    | **Optional**. Limit amount of documents |

---

#### Grep documents in collection

```http
GET /query/${collection}/${id}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `id`          | `string` | **Required**. Id of document    |

---

#### Grep documents in collection by indexes

```http
GET /query/${collection}?limit=${limit}&indexes=${index_name_0},${index_name_1}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `limit`       | `int`    | **Optional**. Limit amount of documents |
| `indexes`        | `string[]` | **Optional**. String array of index names   |

---

#### Create document in collection

```http
POST /query/${collection}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `id`          | `string` | **Required**. Id of document    |
| `data`        | `string` | **Required**. Msgpack data of json data    |

---

#### Update document in collection

```http
PATCH /query/${collection}/${id}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `id`          | `string` | **Required**. Id of document    |
| `data`        | `string` | **Required**. Msgpack data of json data    |


## Contributing

Contributions are always welcome!

See `contributing.md` for ways to get started.

Please adhere to this project's `code of conduct`.



<h1 align="center">Contributors' Wall</h1>

<a href="https://github.com/grimfilerino/random-db/graphs/contributors">
    <img src="https://contri.buzz/api/wall?repo=grimfilerino/random-db" alt="Contributors' Wall for grimfilerino/random-db" />
</a>

<br />
<br />
    
<p align="center">
    Make your Contributors' Wall with <a href="https://contri.buzz/"><i>Contri.Buzz</i></a>
</p>



