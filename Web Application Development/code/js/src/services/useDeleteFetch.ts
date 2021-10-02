import { useEffect, useReducer } from 'react'

type State =
  {
    type: 'started'
  }
  | {
    type: 'fetching'
  }
  | {
    type: 'error',
    error: string
  }
  | {
    type: 'response',
    status: number,
    body: string
  }

type Action =
  {
    type: 'fetching'
  }
  | {
    type: 'error',
    error: string
  }
  | {
    type: 'response',
    status: number,
    body: string
  }

function actionError(error: string): Action {
  return { type: 'error', error: error }
}

function actionResponse(status: number, body: string): Action {
  return { type: 'response', status: status, body: body }
}

function reducer(state: State, action: Action): State {

  switch (action.type) {
    case 'fetching': return { type: 'fetching' }
    case 'error': return { type: 'error', error: action.error }
    case 'response': return { type: 'response', status: action.status, body: action.body }
  }
}


export function useDeleteFetch(uri: string, bodyData: any): State {
  const [state, dispatcher] = useReducer(reducer, { type: 'started' })
  console.log("useDeleteFetch ")
  useEffect(() => {
    console.log("useDeleteFetch useEffect")


    if (!uri) {
      return
    }
    if(bodyData == null){
      return
    }

    let isCancelled = false
    const abortController = new AbortController()
    const signal = abortController.signal
    async function doFetch() {
      try {
        const credentialsJSON = sessionStorage.getItem("CredentialsKey")
        const credentials =  JSON.parse(credentialsJSON)

        let headers = new Headers();

        headers.append('Authorization', credentials.type +" "+ credentials.content.value);
        headers.append('Content-Type', 'application/json')
        
        dispatcher({ type: 'fetching' })
        const resp = await fetch(uri, {
            method: 'DELETE',
            headers: headers, 
            signal,
            body: JSON.stringify(bodyData)
        })
        if (isCancelled) {
          return
        }
        const body = await resp.text()
        if (isCancelled) {
          return
        }
        dispatcher(actionResponse(resp.status, body))
      } catch (error) {
        if (isCancelled) {
          return
        }
        dispatcher(actionError(error.message))
      }
    }
    doFetch()
    return () => {
      isCancelled = true
      abortController.abort()
    }
  }, [uri, bodyData])

  return state
}

