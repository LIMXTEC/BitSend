# Bittrex Public API wrapper
import requests

class pubBittrex:
	def __init__(self):
		self.coin = "bitsend"
		self.summeryURL = "https://bittrex.com/api/v1.1/public/getmarketsummary?market=btc-"

	def getSummery(self):
		
		path = self.summeryURL + self.coin

		newData = requests.get(path).json()

		print(newData)



b = pubBittrex()
b.getSummery()
