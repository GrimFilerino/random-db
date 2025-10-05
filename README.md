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

#### Grep one document in collection

```http
GET /query/${collection}/${_id}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `_id`          | `string` | **Required**. _id of document    |

---

#### Grep documents in collection by indexes

```http
GET /query/${collection}?limit=${limit}&indexes=${index_name_0},${index_name_1}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `limit`       | `int`    | **Optional**. Limit amount of documents |
| `indexes`     | `string` | **Optional**. Index name previously created |

---

#### Create document in collection

```http
POST /query/${collection}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `data`        | `object` | **Required**. JSON data of document to create |

---

#### Update document in collection

```http
PATCH /query/${collection}/${_id}?_rev=${_rev}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `_id`         | `string` | **Required**. _id of document   |
| `_rev`        | `string` | **Required**. _rev of document  |
| `data`        | `object` | **Required**. JSON object       |


Data object structure
```
{
    #set:{ //json data to update and/or add },
    #delete: [ //keys to delete (type `string`) ],
}
```

---

#### Query document in collection

```http
POST /query/${collection}/${_id}?_rev=${_rev}
```

| Parameter     | Type     | Description                     |
| :------------ | :------- | :------------------------------ |
| `collection`  | `string` | **Required**. Collection name   |
| `_id`         | `string` | **Required**. _id of document    |
| `_rev`        | `string` | **Optional**. _rev of document    |
| `pipeline`    | `object[]` | **Required**. Query pipeline|


Pipeline array structure
```
[
    { #where : {} },
    { #add : {} },
    { #filter : {} },
    { #combine : {}}
]
```
---

## Aggregation

### \#where

`#where` is used to filter documents based on certain criterias like the following 
```
#where : {
    _id : ${_id}
}
```

`!where` also has certain keywords that makes quering easier.

```
#where : {
    ${key}: {
        #equals : ${value},
        #less : ${value},
        #more : ${value},
        #lessEquals : ${value},
        #moreEquals : ${value},
        #exsits : (true/false),
        #isEmpty : ${value}
        #isNull : ${value}
        #in : [${value}, ${value}]
        #contains: ${value},
        #startsWith : ${value},
        #endsWith : ${value},
        #like: {
            #value : ${value}
            #percentage: (0.0-1.0)
            #strict: (true/false)
        }
        #and : [],
        #or: [],
        #not: {
            #isEmpty : ${value}
        }
    },
}
```

| Keyword       | Description                     |
| :------------ | :------------------------------ |
| `#equals`     | Checks if key value equals inputed value |
| `#less`       | Checks if key value is less than inputed value |
| `#more`       | Checks if key value is more than inputed value |
| `#lessEquals` | Checks if key value is less than or equal inputed value |
| `#moreEquals` | Checks if key value is more than or equal inputed value |
| `#exsits`     | Checks if key exsits on document |
| `#isEmpty`    | Checks if key of type `array` or `object` is empty |
| `#isNull`     | Checks if key value is null |
| `#in`         | Checks if key value contains inputed value. Only for type `array` |
| `#contains`   | Checks if key of type `string` value contains inputed value |
| `#startsWith` | Checks if key value starts with inputed value |
| `#endsWith`   | Checks if key value ends with inputed value |
| `#like`       | Checks if inputed `string` is similar to key value `string` to a certain percentage |
| `#and`        | Combines two `#where` queries and both must be truthy |
| `#or`         | Combines two `#where` queries and either must be truthy |
| `#not`        | Negates any `#where` query inside object |

---

## Donate to me :D

[![Buy me a beer](https://img.shields.io/badge/buy_me_beer-grimfilerino?style=for-the-badge&logo=homebrew&logoColor=black&logoSize=200&labelColor=fa9b0e&color=fa9b0e)](https://donate.stripe.com/28obKL8sbbgy0ow8ww)

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

